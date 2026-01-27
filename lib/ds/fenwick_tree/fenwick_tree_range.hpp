#pragma once

#include "lib/debug.hpp"

template <typename T>
struct FenwickTreeRange {
  int n = 0;
  std::vector<T> t1, t2;

  FenwickTreeRange() = default;
  explicit FenwickTreeRange(int _n) { resize(_n); }

  void resize(int _n) {
    CHECK(_n >= 0);
    n = _n;
    t1.assign(n + 1, T{});
    t2.assign(n + 1, T{});
  }

  static void add_one(std::vector<T>& t, int n, int x, const T& v) {
    for (; x <= n; x += x & -x) {
      t[x] += v;
    }
  }

  static T sum_one(const std::vector<T>& t, int x) {
    T res{};
    for (; x > 0; x &= x - 1) {
      res += t[x];
    }
    return res;
  }

  void add(int l, int r, const T& v) {
    CHECK(0 <= l && l <= r && r <= n);
    ++l; ++r;
    add_one(t1, n, l, v);
    add_one(t1, n, r, -v);
    add_one(t2, n, l, -v * (l - 1));
    add_one(t2, n, r,  v * (r - 1));
  }

  T sum(int x) const {
    CHECK(0 <= x && x <= n);
    T s1 = sum_one(t1, x);
    T s2 = sum_one(t2, x);
    return s1 * x + s2;
  }

  T sum(int l, int r) const {
    CHECK(0 <= l && l <= r && r <= n);
    return sum(r) - sum(l);
  }

  void reset() {
    std::fill(t1.begin(), t1.end(), T{});
    std::fill(t2.begin(), t2.end(), T{});
  }
};
