#include <thread>
#include <print>
#include <mutex>
#include <chrono>
#include <condition_variable>

std::condition_variable_any cv;
std::mutex m;
bool ready{};

using namespace std::chrono_literals;

/* Check what happend if we wait until time in past */

void worker_thread1(std::chrono::seconds timeout)
{
    std::println("Worker thread1 started {}",std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now()));
    // wait until main() sends data
    std::unique_lock lk(m);
    cv.wait_until(lk,std::chrono::system_clock::now() + timeout, []{ return ready; });
 
    // after the wait, we own the lock
    std::println("Worker thread1 leave wait_until {}",std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now()));

    // manual unlocking is done before notifying, to avoid waking up
    // the waiting thread only to block again (see notify_one for details)
    lk.unlock();
    cv.notify_one();
}

void worker_thread2(std::chrono::seconds timeout)
{
    std::println("Worker thread2 started {}",std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now()));
    // wait until main() sends data
    std::unique_lock lk(m);
    cv.wait_for(lk,timeout, []{ return ready; });
 
    // after the wait, we own the lock
    std::println("Worker thread2 leave wait_for {}",std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now()));

    // manual unlocking is done before notifying, to avoid waking up
    // the waiting thread only to block again (see notify_one for details)
    lk.unlock();
    cv.notify_one();
}

void help()
{
    std::println("### Help ###");
    std::println("example:");
    std::println("executable [1(worker_thread1)|2(worker_thread2)] [timeout]");
    std::println("./build/conditional_variable 1 60");
    std::println("default: worker_thread1, 60s");
}

int main(int argv, char** args)
{
    if (argv == 3)
    {
        std::println("arg1 = {}, arg2 = {}",args[1],args[2]);
        if (std::string_view(args[1]) == "--help")
        {
            help();
            exit(EXIT_SUCCESS);
        }

        std::string_view arg1{args[1]};
        std::string_view arg2{args[2]};

        int thread_number{};
        auto [ptr1, ec1] = std::from_chars(arg1.data(), arg1.data() + arg1.size(), thread_number);
        
        if (ec1 != std::errc{}) {
            // Handle error (invalid input, out-of-range, etc.)
            throw std::runtime_error("Failed to parse arg1 as an integer");
        }

        int seconds{};
        auto [ptr2, ec2] = std::from_chars(arg2.data(), arg2.data() + arg2.size(), seconds);
        
        if (ec2 != std::errc{}) {
            // Handle error (invalid input, out-of-range, etc.)
            throw std::runtime_error("Failed to parse arg2 as an integer");
        }

    std::jthread worker;

    std::println("{} {}",worker.get_id(),worker.joinable());

    

    worker=std::jthread(thread_number == 2 ? worker_thread2 : worker_thread1,std::chrono::seconds(seconds));


    }
    else
    {
        std::println("Not valid amount of arguments");
        help();
        return 1;
    }

    // send data to the worker thread
    // {
    //     std::lock_guard lk(m);
    //     ready = true;
    //     std::println("main() signals data ready for processing");
    // }
    // cv.notify_one();

    return 0;
}

