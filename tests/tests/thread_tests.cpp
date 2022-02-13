#include <catch.hpp>

#include <nod/nod.hpp>

#include <atomic>
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

TEST_CASE("Concurrent disconnection does not deadlock", "[thread_test]")
{
    for (int run = 0; run < 100; run++)
    {
        INFO("Run #" << run);

        nod::signal<void()> sig;
        nod::connection conn;

        conn = sig.connect([] {});

        std::atomic_bool go(false);
        std::atomic<int> ready(0);
        std::atomic<int> finished(0);

        // Use yield() to keep the threads relatively hot on the CPU

        // t1 performs a disconnect() call from a connection object
        std::thread t1([&] {
            ready++;
            while (!go)
            {
                std::this_thread::yield();
            }
            conn.disconnect();
            finished++;
        });

        // t2 performs a call to disconnect_all_slots() on the signal
        std::thread t2([&] {
            ready++;
            while (!go)
            {
                std::this_thread::yield();
            }
            sig.disconnect_all_slots();
            finished++;
        });

        std::chrono::steady_clock::time_point const startPoint = std::chrono::steady_clock::now();

        auto waitTimeExpired = [&startPoint] {
            return (std::chrono::steady_clock::now() - startPoint) > std::chrono::seconds(1);
        };

        auto threadsReady = [&ready] { return ready == 2; };

        auto hasFinished = [&finished] { return finished == 2; };

        // Wait for both threads to be running
        while (!waitTimeExpired() && !threadsReady())
        {
            std::this_thread::yield();
        }

        REQUIRE(!waitTimeExpired());

        // Signal that the threads can now execute their logic
        go = true;

        // Wait for both threads to complete
        while (!waitTimeExpired() && !hasFinished())
        {
            std::this_thread::yield();
        }

        REQUIRE(hasFinished());

        t1.join();
        t2.join();
    }
}

TEST_CASE("Concurrent disconnection and destruction does not deadlock", "[thread_test]")
{
    for (int run = 0; run < 100; run++)
    {
        INFO("Run #" << run);

        std::shared_ptr<nod::signal<void()>> sig = std::make_shared<nod::signal<void()>>();
        nod::connection conn;

        conn = sig->connect([] {});

        std::atomic_bool go(false);
        std::atomic<int> ready(0);
        std::atomic<int> finished(0);

        // Use yield() to keep the threads relatively hot on the CPU

        // t1 performs a disconnect() call from a connection object
        std::thread t1([&] {
            ready++;
            while (!go)
            {
                std::this_thread::yield();
            }
            conn.disconnect();
            finished++;
        });

        // t2 performs the destruction of the signal object
        std::thread t2([&] {
            ready++;
            while (!go)
            {
                std::this_thread::yield();
            }
            sig.reset();
            finished++;
        });

        std::chrono::steady_clock::time_point startPoint = std::chrono::steady_clock::now();

        auto waitTimeExpired = [startPoint = std::chrono::steady_clock::now()] {
            return (std::chrono::steady_clock::now() - startPoint) > std::chrono::seconds(1);
        };

        auto threadsReady = [&ready] { return ready == 2; };

        auto hasFinished = [&finished] { return finished == 2; };

        // Wait for both threads to be running
        while (!waitTimeExpired() && !threadsReady())
        {
            std::this_thread::yield();
        }

        REQUIRE(!waitTimeExpired());

        // Signal that the threads can now execute their logic
        go = true;

        // Wait for both threads to complete
        while (!waitTimeExpired() && !hasFinished())
        {
            std::this_thread::yield();
        }

        REQUIRE(hasFinished());

        t1.join();
        t2.join();
    }
}

TEST_CASE("Concurrent disconnection and connection does not deadlock", "[thread_test]")
{
    for (int run = 0; run < 100; run++)
    {
        INFO("Run #" << run);

        nod::signal<void()> sig;
        nod::connection conn;

        std::atomic_bool go(false);
        std::atomic<int> ready(0);
        std::atomic<int> finished(0);

        // Use yield() to keep the threads relatively hot on the CPU

        // t1 performs a connect() call on the signal
        std::thread t1([&] {
            ready++;
            while (!go)
            {
                std::this_thread::yield();
            }
            conn = sig.connect([] {});
            finished++;
        });

        // t2 performs a call to disconnect_all_slots() on the signal
        std::thread t2([&] {
            ready++;
            while (!go)
            {
                std::this_thread::yield();
            }
            sig.disconnect_all_slots();
            finished++;
        });

        auto waitTimeExpired = [startPoint = std::chrono::steady_clock::now()] {
            return (std::chrono::steady_clock::now() - startPoint) > std::chrono::seconds(1);
        };

        auto threadsReady = [&ready] { return ready == 2; };

        auto hasFinished = [&finished] { return finished == 2; };

        // Wait for both threads to be running
        while (!waitTimeExpired() && !threadsReady())
        {
            std::this_thread::yield();
        }

        REQUIRE(!waitTimeExpired());

        // Signal that the threads can now execute their logic
        go = true;

        // Wait for both threads to complete
        while (!waitTimeExpired() && !hasFinished())
        {
            std::this_thread::yield();
        }

        REQUIRE(hasFinished());

        conn.disconnect();

        t1.join();
        t2.join();
    }
}
