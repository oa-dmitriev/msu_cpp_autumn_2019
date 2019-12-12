#include <iostream>
#include <functional>
#include <vector>
#include <thread>
#include <condition_variable>
#include <queue>
#include <future>
#include <memory>

class ThreadPool {
public:
    using Task = std::function<void ()>;
    explicit ThreadPool(size_t poolSize) {
        for (auto i = 0u; i < poolSize; ++i) {
            mThreads_.emplace_back([=] {
                while (true) {
                    Task task;
                    {
                        std::unique_lock<std::mutex> lock(mEventMutex_);
                        mEventVar_.wait(lock, [=] { 
                            return mStop_ || !mTasks_.empty(); 
                        });
                        if (mStop_ && mTasks_.empty()) {
                            break;
                        }
                        task = std::move(mTasks_.front());
                        mTasks_.pop();
                    }
                    task();
                }
            });
        }
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(mEventMutex_);
            mStop_ = true;
        }
        mEventVar_.notify_all();
        for (auto &thread : mThreads_) {
            thread.join();
        }
    }

    template <class Func, class... Args>
    auto exec(Func func, Args... args)->std::future<decltype(func(args...))> {
        auto wrapper = std::make_shared<
                std::packaged_task<decltype(func(args...)) (Args...)>
                >(std::move(func));
        {
            std::unique_lock<std::mutex> lock(mEventMutex_);
            mTasks_.emplace([=] {
                (*wrapper)(args...);
            });
        }
        mEventVar_.notify_one();
        return wrapper->get_future();
    }
private:
    std::vector<std::thread> mThreads_;
    std::condition_variable mEventVar_;
    std::mutex mEventMutex_;
    bool mStop_ = false;
    std::queue<Task> mTasks_;
};

struct A {};

void foo(const A&) {}

int main() {
    {
        ThreadPool pool(8);

        auto task1 = pool.exec(foo, A());
        task1.get();

        auto task2 = pool.exec([]() { return 1; });
        task2.get();
    }
    return 0;
}