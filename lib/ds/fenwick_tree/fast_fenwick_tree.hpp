#pragma once

#include "lib/debug.hpp"

// Based on https://en.algorithmica.org/hpc/data-structures/segment-trees/#fenwick-trees
template <typename T>
struct FastFenwickTree {
  int n = 0;
  size_t limit = 0;
  std::vector<T> t;
  std::vector<int> bucket;

  static constexpr int hole(int x) {
    return x + (x >> 10);
  }

  FastFenwickTree() = default;
  explicit FastFenwickTree(int _n) { resize(_n); }

  void resize(int _n) {
    CHECK(_n >= 0);
    n = _n;
    limit = n / 20;
    bucket.reserve(limit);
    t.assign(hole(n) + 1, T{});
  }

  void add(int x, const T& v) {
    CHECK(0 <= x && x < n);
    if (bucket.size() < limit) bucket.push_back(x);
    for (++x; x <= n; x += x & -x) {
      t[hole(x)] += v;
    }
  }

  T sum(int x) const {
    CHECK(0 <= x && x <= n);
    T res{};
    for (; x > 0; x &= x - 1) {
      res += t[hole(x)];
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
          t[hole(x)] = T{};
        }
      }
    } else {
      std::fill(t.begin(), t.end(), T{});
    }
    bucket.clear();
  }
};