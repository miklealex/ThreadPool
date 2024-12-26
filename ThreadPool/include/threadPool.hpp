#include <vector>
#include <mutex>
#include <thread>
#include <queue>
#include <condition_variable>
#include <future>
#include <functional>

namespace moleksyn
{
    class ThreadPool
    {
    public:
        static ThreadPool &getInstance(std::size_t threadsAmount = std::thread::hardware_concurrency())
        {
            static ThreadPool instance{threadsAmount};
            return instance;
        }

        template <class T, class... Args>
        auto schedule(T&& func, Args&&... args) -> std::future<decltype(func(args...))>
        {
            auto f = std::bind(std::forward<T>(func), std::forward<Args>(args)...);
            auto taskPtr = std::make_shared<std::packaged_task<decltype(func(args...))()>>(f);
            auto wrapperFunc = [taskPtr](){(*taskPtr)();};

            {
                std::lock_guard lock{tasksGuard_};
                tasks_.push(wrapperFunc);
                synchronizationCV_.notify_one();
            }

            return taskPtr->get_future();
        }

        ~ThreadPool()
        {
            {
                std::lock_guard lock{tasksGuard_};
                shouldQuit_ = true;
                synchronizationCV_.notify_all();
            }

            for(auto& w : workers_)
            {
                w.join();
            }
        }

    private:
        explicit ThreadPool(std::size_t threadsAmount)
            : shouldQuit_{false}
            , workers_{threadsAmount}
        {
            for(auto i = 0; i < threadsAmount; ++i)
            {
                workers_[i] = std::thread(std::bind(&ThreadPool::work, this));
            }
        }

        void work()
        {
            while(!shouldQuit_)
            {
                std::unique_lock<std::mutex> lk(tasksGuard_);
                synchronizationCV_.wait(lk, [this]{ return !tasks_.empty() || shouldQuit_; });

                if(shouldQuit_ || tasks_.empty())
                {
                    return;
                }

                std::function<void()> currentTask;
                {
                    currentTask = tasks_.front();
                    tasks_.pop();
                }

                currentTask();
            }
        }

        std::vector<std::thread> workers_;
        std::queue<std::function<void()>> tasks_;
        std::condition_variable synchronizationCV_;
        std::mutex tasksGuard_;
        std::mutex cvGuard_;
        bool shouldQuit_;
    };
}
