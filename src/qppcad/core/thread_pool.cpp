#include <qppcad/thread_pool.hpp>

using namespace qpp;
using namespace qpp::cad;

void thread_pool_t::thread_task(thread_pool_t *pool) {

  while (true) {

      if (pool->terminate) break;

      std::unique_lock<std::mutex> jobsLock{ pool->jobs_mutex };

      if (pool->jobs.empty() || pool->paused) {
          ++pool->threads_waiting;
          pool->jobs_available.wait(jobsLock, [&]() {
              return pool->terminate || !(pool->jobs.empty() || pool->paused);
            });
          --pool->threads_waiting;
        }

      if (pool->terminate) break;

      auto job = std::move(pool->jobs.front());
      pool->jobs.pop();

      jobsLock.unlock();

      job();

    }

}

thread_pool_t::thread_pool_t(size_t num_threads) :
  threads_waiting(0),
  terminate(false),
  paused(false) {

  if (num_threads <= 0) num_threads = std::thread::hardware_concurrency();

  threads.reserve(num_threads);
  std::generate_n(std::back_inserter(threads),
                  num_threads,
                  [this]() {return std::thread{ thread_task, this }; });

}

thread_pool_t::~thread_pool_t() {

  clear();

  // tell threads to stop when they can
  terminate = true;
  jobs_available.notify_all();

  // wait for all threads to finish
  for (auto& t : threads) {
      if (t.joinable())
        t.join();
    }

}

std::size_t thread_pool_t::thread_cnt() const {

  return threads.size();

}

std::size_t thread_pool_t::waiting_jobs() const {

  std::lock_guard<std::mutex> jobLock(jobs_mutex);
  return jobs.size();

}

std::vector<std::thread::id> thread_pool_t::ids() const {

  std::vector<std::thread::id> ret(threads.size());
  std::transform(threads.begin(), threads.end(), ret.begin(), [](auto& t) { return t.get_id(); });
  return ret;

}

void thread_pool_t::clear() {

  std::lock_guard<std::mutex> lock{ jobs_mutex };
  while (!jobs.empty()) jobs.pop();

}

void thread_pool_t::pause(bool state) {

  paused = state;
  if (!paused) jobs_available.notify_all();

}

void thread_pool_t::wait() {

  while (threads_waiting != threads.size());

}
