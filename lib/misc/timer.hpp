#pragma once

struct Timer {
  using clock = std::chrono::steady_clock;
  using duration = std::chrono::duration<double, std::milli>;

  clock::time_point start_time;
  std::string name;

  Timer() : name() { reset(); }
  explicit Timer(const char* _name = "") : name(_name) { reset(); }

  void reset() { start_time = clock::now(); }

  double get() const {
    auto end_time = clock::now();
    duration diff = end_time - start_time;
    return diff.count();
  }

  void print() const {
    if (!name.empty()) {
      std::cerr << "[timer] " << name << ": " << get() << " ms" << std::endl;
    } else {
      std::cerr << "[timer] " << get() << " ms" << std::endl;
    }
  }
};

struct ScopeTimer {
  Timer timer;

  explicit ScopeTimer(const char* name = "") : timer(name) {}
  ~ScopeTimer() { timer.print(); }
};