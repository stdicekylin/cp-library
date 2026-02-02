#pragma once

template <typename F>
struct y_combinator_t {
  F func;

  template <typename T>
  explicit y_combinator_t(T&& _func) : func(std::forward<T>(_func)) {}

  template <typename... Args>
  auto operator()(Args&&... args) const {
    return func(std::ref(*this), std::forward<Args>(args)...);
  }
};

template <typename F>
auto y_combinator(F&& f) {
  return y_combinator_t<std::decay_t<F>>(std::forward<F>(f));
};