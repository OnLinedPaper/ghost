#ifndef TIME_H_
#define TIME_H_

#include <chrono>

/*
fairly standard template used for timekeeping, not unlike that from qdbp and
p_lite_tuber. incorporates delta time in case the program's running slow, to
ensure timing stays consistent even if the actual animation becomes choppy.

elapsed ticks are updated once per enginge cycle, to ensure consistent behavior
aming all scripts rendering per cycle and avoid race conditions.
*/

class time {
public:
  ~time() { }

  static time &get() {
    static time instance;
    return instance;
  }

  //return elapsed ticks since program start
  int get_tick() { return total_ticks; }

  void update();

  double get_wait() const;
  static double get_ms() {
    return
      std::chrono::system_clock::now().time_since_epoch() /
      std::chrono::milliseconds(1)
    ;
  }

private:
  const int TPS;
  const double T_DELAY;
  int total_ticks;

  const double first_clock;
  double last_clock;
  double delta;

  time() :
      TPS(24)
    , T_DELAY(1000.0/(double)TPS)
    , total_ticks(0)
    , first_clock(get_ms())
    , last_clock(first_clock)
    , delta(0)
  { }
  time(const time&) = delete;
  time &operator=(const time&) = delete;
};

#endif
