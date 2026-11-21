#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include "servermaxheap.hpp"
#include "server.hpp"
#include "common.hpp"

static int passed = 0, failed = 0;
Server *EMPTY_RESULT = nullptr;

Server *addServer(std::vector<std::unique_ptr<Server>> &pool, int id, int cap, int load)
{
    pool.push_back(std::make_unique<Server>(id, cap));
    pool.back()->setLoadForTest(load); // BYPASS the missing Request module!
    return pool.back().get();
}
void expectOrder(const std::string &name, ServerMaxHeap &heap, const std::vector<int> &expected)
{
    std::vector<int> got;
    while (!heap.empty())
    {
        Server *srv = heap.extractMax();
        if (srv != nullptr)
        {
            got.push_back(srv->getId());
        }
    }
    bool ok = (got == expected);
    std::cout << (ok ? "PASS " : "FAIL ") << name << "\n  expected:";
    for (int id : expected)
        std::cout << " " << id;
    std::cout << "\n  got:     ";
    for (int id : got)
        std::cout << " " << id;
    std::cout << "\n";
    ok ? passed++ : failed++;
  
}
void check(const std::string &name, bool ok)
{
    std::cout << (ok ? "PASS " : "FAIL ") << name << "\n";
    ok ? passed++ : failed++;
}

int main()
{
    // 1. Different max capacity, zero load: available = 100, 200, 150
    {
        std::vector<std::unique_ptr<Server>> pool;
        ServerMaxHeap h;
        h.insert(addServer(pool, 1, 100, 0));
        h.insert(addServer(pool, 2, 200, 0));
        h.insert(addServer(pool, 3, 150, 0));
        expectOrder("1: capacity differs", h, {2, 3, 1});
    }
    // 2. Same capacity, different load: available = 5, 8, 2
    {
        std::vector<std::unique_ptr<Server>> pool;
        ServerMaxHeap h;
        h.insert(addServer(pool, 1, 10, 5));
        h.insert(addServer(pool, 2, 10, 2));
        h.insert(addServer(pool, 3, 10, 8));
        expectOrder("2: load differs", h, {2, 1, 3});
    }
    // 3a. Tie: all available = 3, different caps/loads, inserted 1,2,3
    {
        std::vector<std::unique_ptr<Server>> pool;
        ServerMaxHeap h;
        h.insert(addServer(pool, 1, 5, 2));
        h.insert(addServer(pool, 2, 4, 1));
        h.insert(addServer(pool, 3, 3, 0));
        expectOrder("3a: tie, inserted 1,2,3", h, {1, 2, 3});
    }
    // 3b. Same tie, inserted 3,2,1
    {
        std::vector<std::unique_ptr<Server>> pool;
        ServerMaxHeap h;
        h.insert(addServer(pool, 3, 3, 0));
        h.insert(addServer(pool, 2, 4, 1));
        h.insert(addServer(pool, 1, 5, 2));
        expectOrder("3b: tie, inserted 3,2,1", h, {1, 2, 3});
    }
    // 4. Multi-level climb: available = 5, 4, 3, then 9 rises to the root
    {
        std::vector<std::unique_ptr<Server>> pool;
        ServerMaxHeap h;
        h.insert(addServer(pool, 1, 10, 5));
        h.insert(addServer(pool, 2, 10, 6));
        h.insert(addServer(pool, 3, 10, 7));
        h.insert(addServer(pool, 4, 10, 1));
        expectOrder("4: multi-level climb", h, {4, 1, 2, 3});
    }
    // 5. Six servers, scrambled, with a tie (ids 3 and 4 both available 5)
    //    available: id1=7, id2=1, id3=5, id4=5, id5=10, id6=3
    {
        std::vector<std::unique_ptr<Server>> pool;
        ServerMaxHeap h;
        h.insert(addServer(pool, 1, 10, 3));
        h.insert(addServer(pool, 2, 10, 9));
        h.insert(addServer(pool, 3, 10, 5));
        h.insert(addServer(pool, 4, 10, 5));
        h.insert(addServer(pool, 5, 10, 0));
        h.insert(addServer(pool, 6, 10, 7));
        expectOrder("5: full drain with tie", h, {5, 1, 3, 4, 6, 2});
    }
    // 6. Extract from an empty heap
    {
        ServerMaxHeap h;
        Server *r = h.extractMax();
        bool ok = (r == EMPTY_RESULT);
        std::cout << (ok ? "PASS " : "FAIL ") << "6: extract on empty, returned " << (r ? std::to_string(r->getId()) : "nullptr") << "\n";
        ok ? passed++ : failed++;
    }

    
    // 7. Single element
    {
        std::vector<std::unique_ptr<Server>> pool;
        ServerMaxHeap h;
        h.insert(addServer(pool, 7, 10, 0));
        expectOrder("7: single element", h, {7});
    }
    // 8. All five servers tied (available = 4), inserted out of order
    {
        std::vector<std::unique_ptr<Server>> pool;
        ServerMaxHeap h;
        for (int id : {5, 3, 1, 4, 2})
            h.insert(addServer(pool, id, 10, 6));
        expectOrder("8: all tied", h, {1, 2, 3, 4, 5});
    }
    // 9. Ascending availability (1..5): every insert climbs to the root
    {
        std::vector<std::unique_ptr<Server>> pool;
        ServerMaxHeap h;
        for (int id = 1; id <= 5; id++)
            h.insert(addServer(pool, id, 10, 10 - id));
        expectOrder("9: ascending inserts", h, {5, 4, 3, 2, 1});
    }
    // 10. Descending availability (5..1): no insert swaps at all
    {
        std::vector<std::unique_ptr<Server>> pool;
        ServerMaxHeap h;
        for (int id = 1; id <= 5; id++)
            h.insert(addServer(pool, id, 10, 4 + id));
        expectOrder("10: descending inserts", h, {1, 2, 3, 4, 5});
    }
    // 11. Interleaved insert and extract
    {
        std::vector<std::unique_ptr<Server>> pool;
        ServerMaxHeap h;
        h.insert(addServer(pool, 1, 10, 5)); // available 5
        h.insert(addServer(pool, 2, 10, 2)); // available 8
        Server *first = h.extractMax();
        check("11a: first extract is id 2", first && first->getId() == 2);
        h.insert(addServer(pool, 3, 10, 1)); // available 9
        h.insert(addServer(pool, 4, 10, 5)); // available 5, ties with id 1
        expectOrder("11b: rest after more inserts", h, {3, 1, 4});
    }
    // 12. Full and overloaded servers: available 0 and -2 must rank last
    {
        std::vector<std::unique_ptr<Server>> pool;
        ServerMaxHeap h;
        h.insert(addServer(pool, 1, 10, 10)); // available 0
        h.insert(addServer(pool, 2, 10, 4));  // available 6
        h.insert(addServer(pool, 3, 10, 12)); // available -2
        expectOrder("12: full and overloaded", h, {2, 1, 3});
    }
    // 13. Stale order: load changes while the server is inside the heap
    {
        std::vector<std::unique_ptr<Server>> pool;
        ServerMaxHeap h;
        Server *a = addServer(pool, 1, 10, 2); // available 8
        Server *b = addServer(pool, 2, 10, 5); // available 5
        h.insert(a);
        h.insert(b);
        a->setLoadForTest(9); // a now has available 1, heap not told
        Server *top = h.extractMax();
        check("13: stale order (documents a limitation)", top && top->getId() == 1);
    }
    // 14. 100 servers, many ties: drain must be sorted
    {
        std::vector<std::unique_ptr<Server>> pool;
        ServerMaxHeap h;
        for (int i = 1; i <= 100; i++)
            h.insert(addServer(pool, i, 100, (i * 37 + 11) % 20));
        bool ok = true;
        int prevAvail = 1000, prevId = 0, count = 0;
        while (!h.empty())
        {
            Server *s = h.extractMax();
            int av = s->getMaxCapacity() - s->getCurrentLoad();
            if (av > prevAvail || (av == prevAvail && s->getId() < prevId))
                ok = false;
            prevAvail = av;
            prevId = s->getId();
            count++;
        }
        check("14: 100-server drain is sorted", ok && count == 100);
    }
    std::cout << "\n"
              << passed << " passed, " << failed << " failed\n";
    return failed == 0 ? 0 : 1;
}