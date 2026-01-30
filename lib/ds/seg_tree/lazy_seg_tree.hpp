#pragma once

#include "lib/debug.hpp"
#include "lib/math/my_bit.hpp"

template <typename T>
struct LazySegTree {
  using Info = typename T::Info;
  using Tag  = typename T::Tag;

  int n = 0;
  int m = 0;
  int h = 0;
  std::vector<Info> t;
  std::vector<Tag> tag;
  std::vector<uint8_t> has_tag;

  LazySegTree() = default;
  explicit LazySegTree(int _m) { build(_m); }

  template <typename F>
  LazySegTree(int _m, F&& func) { build(_m, func); }

  template <typename It>
  LazySegTree(It first, It last) { build(first, last); }

  void build(int _m) {
    m = _m;
    CHECK(m >= 0);
    n = 1;
    while (n < m) n <<= 1;
    h = my_bit::__lg(n);
    t.assign(n << 1, T::id());
    tag.assign(n << 1, T::tag_id());
    has_tag.assign(n << 1, 0);
  }

  template <typename F>
  void build(int _m, F&& func) {
    m = _m;
    CHECK(m >= 0);
    n = 1;
    while (n < m) n <<= 1;
    h = my_bit::__lg(n);
    t.assign(n << 1, T::id());
    tag.assign(n << 1, T::tag_id());
    has_tag.assign(n << 1, 0);
    for (int i = 0; i < m; ++i) t[n + i] = func(i);
    for (int i = n - 1; i > 0; --i) push_up(i);
  }

  template <typename It>
  void build(It first, It last) {
    m = std::distance(first, last);
    CHECK(m >= 0);
    n = 1;
    while (n < m) n <<= 1;
    h = my_bit::__lg(n);
    t.assign(n << 1, T::id());
    tag.assign(n << 1, T::tag_id());
    has_tag.assign(n << 1, 0);
    std::copy(first, last, t.begin() + n);
    for (int i = n - 1; i > 0; --i) push_up(i);
  }

  void reset() {
    std::fill(t.begin(), t.end(), T::id());
    std::fill(tag.begin(), tag.end(), T::tag_id());
    std::fill(has_tag.begin(), has_tag.end(), 0);
  }

  void set(int x, const Info& v) {
    CHECK(0 <= x && x < m);
    x += n;
    for (int i = h; i > 0; --i) push_down(x >> i);
    t[x] = v;
    while (x >>= 1) push_up(x);
  }

  Info get(int x) {
    CHECK(0 <= x && x < m);
    x += n;
    Info res = t[x];
    while (x >>= 1) T::apply(res, tag[x]);
    return res;
  }

  void apply(int l, int r, const Tag& v) {
    CHECK(0 <= l && l <= r && r <= m);
    if (l == r) return;
    l += n, r += n;
    int w = my_bit::__lg(l ^ r);
    int cl = my_bit::countr_zero(l);
    int cr = my_bit::countr_zero(r);

    for (int i = h; i > cl; --i) push_down(l >> i);
    for (int i = std::max(cl, w); i > cr; --i) push_down(r >> i);

    int L = l, R = r;
    int k = my_bit::__lg(--L ^ R);

    int cL = L;
    L = ~L & ((1 << k) - 1);
    while (L > 0) {
      int i = my_bit::countr_zero(L);
      L ^= 1 << i;
      apply_node(cL >> i ^ 1, v);
    }

    int cR = R;
    R &= (1 << k) - 1;
    while (R > 0) {
      int i = my_bit::__lg(R);
      R ^= 1 << i;
      apply_node(cR >> i ^ 1, v);
    }

    for (int i = cl + 1; i <= w; ++i) push_up(l >> i);
    for (int i = cr + 1; i <= w; ++i) push_up(r >> i);
    for (l >>= w + 1; l > 0; l >>= 1) push_up(l);
  }

  Info prod(int l, int r) {
    CHECK(0 <= l && l <= r && r <= m);
    if (l == r) return T::id();
    Info left = T::id(), right = T::id();
    l += n - 1, r += n;
    while ((l ^ r) != 1) {
      if (~l & 1) left = T::op(left, t[l ^ 1]);
      if (r & 1) right = T::op(t[r ^ 1], right);
      l >>= 1, r >>= 1;
      T::apply(left, tag[l]);
      T::apply(right, tag[r]);
    }

    Info sum = T::op(left, right);
    while (l >>= 1) T::apply(sum, tag[l]);
    return sum;
  }

  Info all_prod() const { return t[1]; }

 private:
  void push_up(int x) {
    t[x] = T::op(t[x << 1], t[x << 1 | 1]);
  }

  void apply_node(int x, const Tag& v) {
    has_tag[x] = 1;
    T::apply(t[x], v);
    T::compose(tag[x], v);
  }

  void push_down(int x) {
    if (has_tag[x]) {
      apply_node(x << 1, tag[x]);
      apply_node(x << 1 | 1, tag[x]);
      tag[x] = T::tag_id();
      has_tag[x] = 0;
    }
  }
};