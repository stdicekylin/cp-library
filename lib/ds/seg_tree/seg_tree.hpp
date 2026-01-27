#pragma once

#include "lib/debug.hpp"
#include "lib/math/my_bit.hpp"

template <typename T>
struct SegTree {
  using Info = typename T::Info;

  int n = 0;
  int size = 0;
  std::vector<Info> t;

  SegTree() = default;
  explicit SegTree(int _n) { build(_n); }

  template <typename It>
  SegTree(It first, It last) { build(first, last); }

  void build(int _n) {
    CHECK(_n >= 0);
    size = _n;
    n = 1;
    while (n < size) n <<= 1;
    t.assign(n << 1, T::id());
  }

  template <typename It>
  void build(It first, It last) {
    size = std::distance(first, last);
    CHECK(size >= 0);
    n = 1;
    while (n < size) n <<= 1;
    t.assign(n << 1, T::id());
    std::copy(first, last, t.begin() + n);
    for (int i = n - 1; i > 0; --i) push_up(i);
  }

  void reset() {
    std::fill(t.begin(), t.end(), T::id());
  }

  void set(int x, const Info& v) {
    CHECK(0 <= x && x < size);
    t[x += n] = v;
    while (x >>= 1) push_up(x);
  }

  Info get(int x) const {
    CHECK(0 <= x && x < size);
    return t[n + x];
  }

  Info prod(int l, int r) const {
    CHECK(0 <= l && l <= r && r <= size);
    Info left = T::id(), right = T::id();
    for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
      if (l & 1) left = T::op(left, t[l++]);
      if (r & 1) right = T::op(t[--r], right);
    }
    return T::op(left, right);
  }

  Info all_prod() const { return t[1]; }

 private:
  void push_up(int x) {
    t[x] = T::op(t[x << 1], t[x << 1 | 1]);
  }
};