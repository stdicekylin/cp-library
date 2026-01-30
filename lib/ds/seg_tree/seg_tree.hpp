#pragma once

#include "lib/debug.hpp"
#include "lib/math/my_bit.hpp"

template <typename T>
struct SegTree {
  using Info = typename T::Info;

  int n = 0;
  int m = 0;
  std::vector<Info> t;

  SegTree() = default;
  explicit SegTree(int _m) { build(_m); }

  template <typename F>
  SegTree(int _m, F&& func) { build(_m, func); }

  template <typename It>
  SegTree(It first, It last) { build(first, last); }

  void build(int _m) {
    m = _m;
    CHECK(m >= 0);
    n = 1;
    while (n < m) n <<= 1;
    t.assign(n << 1, T::id());
  }

  template <typename F>
  void build(int _m, F&& func) {
    m = _m;
    CHECK(m >= 0);
    n = 1;
    while (n < m) n <<= 1;
    t.assign(n << 1, T::id());
    for (int i = 0; i < m; ++i) t[n + i] = func(i);
    for (int i = n - 1; i > 0; --i) push_up(i);
  }

  template <typename It>
  void build(It first, It last) {
    m = std::distance(first, last);
    CHECK(m >= 0);
    n = 1;
    while (n < m) n <<= 1;
    t.assign(n << 1, T::id());
    std::copy(first, last, t.begin() + n);
    for (int i = n - 1; i > 0; --i) push_up(i);
  }

  void reset() {
    std::fill(t.begin(), t.end(), T::id());
  }

  void set(int x, const Info& v) {
    CHECK(0 <= x && x < m);
    x += n;
    t[x] = v;
    while (x >>= 1) push_up(x);
  }

  Info get(int x) const {
    CHECK(0 <= x && x < m);
    return t[n + x];
  }

  Info prod(int l, int r) const {
    CHECK(0 <= l && l <= r && r <= m);
    return evaluate(l, r, T::id(), [&](const Info& x, const Info& y) {
      return T::op(x, y);
    });
  }

  Info all_prod() const { return t[1]; }

  template <typename Value, typename F>
  Value evaluate(int l, int r, Value val, F&& func) const {
    CHECK(0 <= l && l <= r && r <= m);
    l += n - 1, r += n;
    int w = my_bit::__lg(l ^ r);

    int cl = l;
    l = ~l & ((1 << w) - 1);
    while (l > 0) {
      int i = my_bit::countr_zero(l);
      l ^= 1 << i;
      val = func(val, t[cl >> i ^ 1]);
    }

    int cr = r;
    r &= (1 << w) - 1;
    while (r > 0) {
      int i = my_bit::__lg(r);
      r ^= 1 << i;
      val = func(val, t[cr >> i ^ 1]);
    }

    return val;
  }

 private:
  void push_up(int x) {
    t[x] = T::op(t[x << 1], t[x << 1 | 1]);
  }
};