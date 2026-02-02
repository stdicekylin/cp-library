#pragma once

struct Timer {
  using clock = std::chrono::steady_clock;
  using duration = std::chrono::duration<double, std::milli>;

  clock::time_point start_time;
  std::string name;

  Timer() : name() { reset(); }
  explicit Timer(const char* _name) : name(_name) { reset(); }

  void reset() { start_time = clock::now(); }

  double get() const {
    auto end_time = clock::now();
    duration diff = end_time - start_time;
    return diff.count();
  }

  void print() const {
#ifdef LOCAL
    if (!name.empty()) {
      std::cerr << "[TIMER] " << name << ": " << get() << " ms\n";
    } else {
      std::cerr << "[TIMER] " << get() << " ms\n";
    }
#endif
  }
};

struct ScopeTimer {
  Timer timer;

  ScopeTimer() = default;
  explicit ScopeTimer(const char* name) : timer(name) {}
  ~ScopeTimer() { timer.print(); }
};