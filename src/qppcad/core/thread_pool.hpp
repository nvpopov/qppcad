#ifndef QPPCAD_THREAD_POOL
#define QPPCAD_THREAD_POOL

#include <qppcad/core/qppcad.hpp>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>

namespace qpp {

  namespace cad {

    /**
     * @brief The thread_pool_t class
     * https://github.com/dabbertorres/ThreadPool/blob/master/ThreadPool.hpp
    */
    class thread_pool_t {

      private:

        // function each thread performs
        static void thread_task(thread_pool_t* pool);

        std::queue<std::function<void()>> jobs;
        mutable std::mutex jobs_mutex;

        // notification variable for waiting threads
        std::condition_variable jobs_available;

        std::vector<std::thread> threads;
        std::atomic<std::size_t> threads_waiting;

        std::atomic<bool> terminate;
        std::atomic<bool> paused;

      public:

        thread_pool_t(size_t num_threads = 1);

        // non-copyable,
        thread_pool_t(const thread_pool_t&) = delete;
        thread_pool_t& operator=(const thread_pool_t&) = delete;

        // movable
        thread_pool_t(thread_pool_t&&) = default;
        thread_pool_t& operator=(thread_pool_t&&) = default;

        ~thread_pool_t();

        // number of threads being used
        std::size_t thread_cnt() const;

        // number of jobs waiting to be executed
        std::size_t waiting_jobs() const;

        std::vector<std::thread::id> ids() const;

        void clear();
        void pause(bool state);
        void wait();

        template<typename Func, typename... Args>
        auto add_task(Func&& func, Args&&... args)->
        std::future<typename std::result_of<Func(Args...)>::type>;

    };

    template<typename Func, typename... Args>
    auto thread_pool_t::add_task(Func&& func, Args&&... args) ->
    std::future<typename std::result_of<Func(Args...)>::type> {

      using packed_task_t = std::packaged_task<typename std::result_of<Func(Args...)>::type()>;

      auto task =
          std::make_shared<packed_task_t>(std::bind(std::forward<Func>(func),
                                                    std::forward<Args>(args)...)
                                          );

      auto ret = task->get_future();

      {
        std::lock_guard<std::mutex> lock{ jobs_mutex };
        jobs.emplace([task]() { (*task)(); });
      }

      jobs_available.notify_one();

      return ret;

    }

  } // namespace cad

} // namespace qpp

#endif
