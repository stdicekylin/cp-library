#pragma once

#include "lib/debug.hpp"

template <typename T>
struct FenwickTree {
  int n = 0;
  size_t limit = 0;
  std::vector<T> t;
  std::vector<int> bucket;

  FenwickTree() = default;
  explicit FenwickTree(int _n) { resize(_n); }

  void resize(int _n) {
    CHECK(_n >= 0);
    n = _n;
    limit = n / 20;
    bucket.reserve(limit);
    t.assign(n + 1, T{});
  }

  void add(int x, const T& v) {
    CHECK(0 <= x && x < n);
    if (bucket.size() < limit) bucket.push_back(x);
    for (++x; x <= n; x += x & -x) {
      t[x] += v;
    }
  }

  T sum(int x) const {
    CHECK(0 <= x && x <= n);
    T res{};
    for (; x > 0; x &= x - 1) {
      res += t[x];
    }
    return res;
  }

  T sum(int l, int r) const {
    CHECK(0 <= l && l <= r && r <= n);
    return sum(r) - sum(l);
  }

  void reset() {
    if (bucket.size() < limit) {
      for (int x : bucket) {
        for (++x; x <= n; x += x & -x) {
          t[x] = T{};
        }
      }
    } else {
      std::fill(t.begin(), t.end(), T{});
    }
    bucket.clear();
  }
};