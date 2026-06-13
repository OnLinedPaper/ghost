#include "timekeep.h"
#include <algorithm>

void time::update() {
  double elapsed_ms = get_ms() - last_clock;
  delta = std::max(elapsed_ms / T_DELAY, 1.0);

  last_clock = get_ms();

  total_ticks = (last_clock - first_clock) / T_DELAY;
}

double time::get_wait() const {
  double remaining_ms = T_DELAY - (get_ms() - last_clock);
  return std::max(remaining_ms, 0.0);
}
