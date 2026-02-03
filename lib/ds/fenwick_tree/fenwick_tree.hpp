#pragma once

#include "lib/utils/debug.hpp"

template <typename T>
struct FenwickTree {
  using Info = typename T::Info;

  int n = 0;
  size_t limit = 0;
  std::vector<Info> t;
  std::vector<int> bucket;

  FenwickTree() = default;
  explicit FenwickTree(int _n) { resize(_n); }

  void resize(int _n) {
    CHECK(_n >= 0);
    n = _n;
    limit = n / 20;
    bucket.reserve(limit);
    t.assign(n + 1, T::id());
  }

  void add(int x, const Info& v) {
    CHECK(0 <= x && x < n);
    if (bucket.size() < limit) bucket.push_back(x);
    for (++x; x <= n; x += x & -x) {
      t[x] = T::op(t[x], v);
    }
  }

  Info sum(int x) const {
    CHECK(0 <= x && x <= n);
    Info res = T::id();
    for (; x > 0; x &= x - 1) {
      res = T::op(res, t[x]);
    }
    return res;
  }

  Info sum(int l, int r) const {
    CHECK(0 <= l && l <= r && r <= n);
    return T::op(sum(r), T::inv(sum(l)));
  }

  void reset() {
    if (bucket.size() < limit) {
      for (int x : bucket) {
        for (++x; x <= n; x += x & -x) {
          t[x] = T::id();
        }
      }
    } else {
      std::fill(t.begin(), t.end(), T::id());
    }
    bucket.clear();
  }
};