#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <future>

class ThreadPool {
public:
    using Task = std::function<void ()>;
    using Lock = std::unique_lock<std::mutex>;

    explicit ThreadPool(size_t poolSize) 
            : poolSize_(poolSize), mStop_(false), count_(0) {
        for (auto i = 0u; i < poolSize_; ++i) {
            auto thread = [=]{
                while (true) {
                    Task task;
                    {
                        Lock lock(m_);
                        cv_.wait(lock, [=] { 
                            return mStop_ || !mTasks_.empty();
                        });
                        if (mStop_ && mTasks_.empty() || count_ > poolSize_) {
                            break;
                        }
                        task = std::move(mTasks_.front());
                        mTasks_.pop();
                    }
                    task();
                }
            };
            mThreads_.emplace_back(thread);	
        }
    }
    
    ~ThreadPool() {
        mStop_ = true;
        cv_.notify_all();	
        for (auto &thread : mThreads_) {
            thread.join();
        }
    }
    
    template <class Func, class... Args>
    auto exec(Func func, Args... args) -> 
            std::future<decltype(func(args...))> {
        using returnType = decltype(func(args...));
        auto wrapper = std::make_shared<std::packaged_task<
                returnType (Args...)>>(std::move(func));
        {
            Lock lock(m_);
            mTasks_.emplace([=] { (*wrapper)(args...); });
        }
        cv_.notify_one();
        ++count_;
        return wrapper->get_future();	
    }
    
private:
    std::mutex m_;
    std::condition_variable cv_;
    std::atomic<bool> mStop_;
    std::atomic<size_t> count_;
    std::vector<std::thread> mThreads_;
    std::queue<Task> mTasks_;
    size_t poolSize_;
};

struct A {};

void foo(const A&) {};

int main() {
    ThreadPool pool(8);
    auto task1 = pool.exec(foo, A());
    task1.get();

    auto task2 = pool.exec([]() { return 1; });
    task2.get();
    return 0;
}