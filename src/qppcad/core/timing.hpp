#ifndef QPPCAD_TIMING
#define QPPCAD_TIMING

#include <iostream>
#include <chrono>

namespace qpp {

namespace cad {

struct timer_t {

  using clock_t = std::chrono::high_resolution_clock;
  using clock_time_t = clock_t::time_point;

  clock_time_t start_time;

  timer_t() {
    reset();
  }

  void reset() {
    start_time = clock_t::now();
  }

  double elapsed() {
    auto end_time = clock_t::now();
    std::chrono::duration<double> dur = end_time - start_time;
    return dur.count();
  }

};


}

}

#endif
