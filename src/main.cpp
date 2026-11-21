#include "httplib.h"
#include <atomic>
#include <chrono>
#include <csignal>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>
#include <iomanip>
#include <vector>
#include "../include/report_generator.hpp"
#include "../include/common.hpp"
#include "../include/clusterManager.hpp"
#include "../include/RequestQueue.hpp"
#include "../include/load_balancer.hpp"
#include "../include/failure_handler.hpp"
#include "../include/server.hpp"

// ============================================================================
//  THREADING MODEL
//  * ONE mutex (sim_mutex) guards ALL shared simulation state: the cluster,
//    the waiting queue, the load balancer's counters and the auto-scaler's
//    streak. None of those classes lock internally.
//  * A background worker advances time: every TICK_INTERVAL_MS it locks,
//    runs lb.processTick(), and unlocks.
//  * Every HTTP handler (the web server runs them on its own threads) takes the
//    SAME mutex before it reads or changes anything.
//  * A plain std::mutex is not re-entrant, so helpers marked "caller holds
//    lock" must not lock again.
// ============================================================================

static const int TICK_INTERVAL_MS = 3000; // time between automatic ticks
static const int SLEEP_SLICE_MS = 100;    // worker checks the stop flag this often
static const int HTTP_PORT = 8080;

static std::mutex sim_mutex;
static std::atomic<bool> running(true);
static int tick_count = 0;      // guarded by sim_mutex
static int next_request_id = 1; // guarded by sim_mutex

static httplib::Server *g_server = nullptr;
static void onSignal(int)
{
    running = false;
    if (g_server)
        g_server->stop();
}

// ----------------------------------------------------------------------------
//  Background worker: one tick per interval.
// ----------------------------------------------------------------------------
void backgroundWorker(Loadbalancer &lb)
{
    while (running)
    {
        int waited = 0;
        while (running && waited < TICK_INTERVAL_MS) // sleep WITHOUT the lock
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_SLICE_MS));
            waited += SLEEP_SLICE_MS;
        }
        if (!running)
            break;

        std::lock_guard<std::mutex> lock(sim_mutex);
        ++tick_count;
        lb.processTick();
    }
}

// ----------------------------------------------------------------------------
//  JSON helpers (caller holds lock). They read through public getters only.
// ----------------------------------------------------------------------------
static char typeChar(RequestType t)
{
    switch (t)
    {
    case RequestType::LIGHT:
        return 'L';
    case RequestType::MEDIUM:
        return 'M';
    default:
        return 'H';
    }
}

static void writeRequest(std::ostringstream &o, const Request &r)
{
    o << "{\"id\":" << r.request_id << ",\"t\":\"" << typeChar(r.type)
      << "\",\"rem\":" << r.remaining_ticks << "}";
}

static std::string buildStateJson(ClusterManager &cm, RequestQueue &rq, Loadbalancer &lb)
{
    std::ostringstream o;
    o << "{\"tick\":" << tick_count
      << ",\"tickMs\":" << TICK_INTERVAL_MS
      << ",\"received\":" << lb.getTotalRequestsReceived()
      << ",\"processed\":" << lb.getTotalRequestsProcessed()
      << ",\"capacity\":" << SystemConfig::MAX_SERVER_CAPACITY
      << ",\"servers\":[";
    std::vector<Server *> servers = cm.getAllServers();
    for (size_t i = 0; i < servers.size(); i++)
    {
        Server *s = servers[i];
        if (i)
            o << ",";
        o << "{\"id\":" << s->getId()
          << ",\"active\":" << (s->getStatus() == ServerStatus::ACTIVE ? "true" : "false")
          << ",\"load\":" << s->getCurrentLoad()
          << ",\"slots\":[";
        bool first = true;
        for (requestNode *n = s->getHead(); n != nullptr; n = n->next)
        {
            if (!first)
                o << ",";
            first = false;
            writeRequest(o, n->data);
        }
        o << "]}";
    }
    o << "],\"queue\":[";
    std::vector<Request> q = rq.snapshot();
    for (size_t i = 0; i < q.size(); i++)
    {
        if (i)
            o << ",";
        writeRequest(o, q[i]);
    }
    o << "]}";
    return o.str();
}

// Caller holds lock. type: 'L', 'M', 'H', or anything else for a random mix.
static Request makeRequest(char type)
{
    if (type != 'L' && type != 'M' && type != 'H')
    {
        int r = std::rand() % 100; // 50% light, 35% medium, 15% heavy
        type = r < 50 ? 'L' : (r < 85 ? 'M' : 'H');
    }
    RequestType t = type == 'L'   ? RequestType::LIGHT
                    : type == 'M' ? RequestType::MEDIUM
                                  : RequestType::HEAVY;
    return Request(next_request_id++, t, tick_count);
}

int main()
{
    std::srand((unsigned)std::time(nullptr));

    ClusterManager cluster;
    RequestQueue queue;
    Loadbalancer lb(&cluster, &queue);

    std::cout << "Booting Smart Cloud Simulator...\n";
    for (int i = 0; i < SystemConfig::INITIAL_SERVER_COUNT; i++)
        cluster.addServer(); // worker not started yet, so no lock needed

    std::thread worker(backgroundWorker, std::ref(lb));

    httplib::Server svr;
    g_server = &svr;
    std::signal(SIGINT, onSignal);
    svr.set_mount_point("/", "./web"); // serves web/index.html and web/client.html

    auto sendState = [&](httplib::Response &res)
    {
        res.set_content(buildStateJson(cluster, queue, lb), "application/json");
    };

    svr.Get("/state", [&](const httplib::Request &, httplib::Response &res)
            {
        std::lock_guard<std::mutex> lock(sim_mutex);
        sendState(res); });
    svr.Get("/report", [&](const httplib::Request &, httplib::Response &res)
            {
    std::lock_guard<std::mutex> lock(sim_mutex);
    ReportGenerator generator;
    std::vector<ServerReport> report = generator.generateReport(cluster.getActiveServers());
    std::ostringstream o;
    o << std::fixed << std::setprecision(1);
    o << "{\"report\":[";
    for (size_t i = 0; i < report.size(); i++)
    {
        if (i)
            o << ",";
        o << "{\"rank\":" << (i + 1)
          << ",\"serverId\":" << report[i].serverId
          << ",\"utilization\":" << report[i].utilization << "}";
    }
    o << "]}";
    res.set_content(o.str(), "application/json"); });
    // POST /request?n=10&type=H   (n defaults to 1, type defaults to a random mix)
    svr.Post("/request", [&](const httplib::Request &req, httplib::Response &res)
             {
        int n = req.has_param("n") ? std::atoi(req.get_param_value("n").c_str()) : 1;
        std::string t = req.has_param("type") ? req.get_param_value("type") : "";
        if (n < 1) n = 1;
        if (n > 100) n = 100;
        std::lock_guard<std::mutex> lock(sim_mutex); // whole batch under ONE lock
        for (int i = 0; i < n; i++)
            lb.receiveRequest(makeRequest(t.empty() ? '?' : t[0]));
        sendState(res); });

    // POST /server/crash?id=2  and  /server/restore?id=2 (uses your FailureHandler)
    svr.Post("/server/crash", [&](const httplib::Request &req, httplib::Response &res)
             {
        int id = std::atoi(req.get_param_value("id").c_str());
        std::lock_guard<std::mutex> lock(sim_mutex);
        FailureHandler handler(id, cluster, queue);
        handler.simulateCrash();
        sendState(res); });

    svr.Post("/server/restore", [&](const httplib::Request &req, httplib::Response &res)
             {
        int id = std::atoi(req.get_param_value("id").c_str());
        std::lock_guard<std::mutex> lock(sim_mutex);
        FailureHandler handler(id, cluster, queue);
        handler.recoverServer();
        sendState(res); });

    // Debug-only endpoints, used by the dashboard's hidden Debug panel.
    // The worker already ticks and the auto-scaler already adds servers.
    svr.Post("/tick", [&](const httplib::Request &, httplib::Response &res)
             {
        std::lock_guard<std::mutex> lock(sim_mutex);
        ++tick_count;
        lb.processTick();
        sendState(res); });

    svr.Post("/server/add", [&](const httplib::Request &, httplib::Response &res)
             {
        std::lock_guard<std::mutex> lock(sim_mutex);
        if (!cluster.isAtMaxCapacity())
            cluster.addServer();
        sendState(res); });

    std::cout << "Open http://localhost:" << HTTP_PORT << " in your browser (Ctrl+C to stop)\n";
    svr.listen("127.0.0.1", HTTP_PORT); // blocks until Ctrl+C

    // Clean shutdown: stop the worker and wait for it (a joinable thread that is
    // destroyed calls std::terminate).
    running = false;
    worker.join();
    std::cout << "Simulator stopped.\n";
    return 0;
}
