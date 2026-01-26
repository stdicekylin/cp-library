#pragma once

#include "lib/math/my_bit.hpp"

template <typename T>
struct LazySegTree {
  using Info = typename T::Info;
  using Tag  = typename T::Tag;

  int n = 0;
  int size = 0;
  int h = 0;
  std::vector<Info> t;
  std::vector<Tag> tag;
  std::vector<uint8_t> has_tag;

  LazySegTree() = default;
  explicit LazySegTree(int _n) { build(_n); }

  template <typename It>
  LazySegTree(It first, It last) { build(first, last); }

  void build(int _n) {
    CHECK(_n >= 0);
    size = _n;
    n = 1;
    while (n < size) n <<= 1;
    h = my_bit::bit_width(n);
    t.assign(n << 1, T::id());
    tag.assign(n << 1, T::tag_id());
    has_tag.assign(n << 1, 0);
  }

  template <typename It>
  void build(It first, It last) {
    size = std::distance(first, last);
    CHECK(size >= 0);
    n = 1;
    while (n < size) n <<= 1;
    h = my_bit::bit_width(n);
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
    CHECK(0 <= x && x < size);
    x += n;
    for (int i = h; i > 0; --i) push_down(x >> i);
    t[x] = v;
    while (x >>= 1) push_up(x);
  }

  Info get(int x) {
    CHECK(0 <= x && x < size);
    x += n;
    for (int i = h; i > 0; --i) push_down(x >> i);
    return t[x];
  }

  void apply(int l, int r, const Tag& v) {
    CHECK(0 <= l && l <= r && r <= size);
    if (l == r) return;
    l += n, r += n;
    int w = my_bit::bit_width(l ^ r);
    int cl = my_bit::countr_zero(l);
    int cr = my_bit::countr_zero(r);

    for (int i = h; i > cl; --i) push_down(l >> i);
    for (int i = std::max(cl, w); i > cr; --i) push_down(r >> i);

    for (int L = l, R = r; L < R; L >>= 1, R >>= 1) {
      if (L & 1) apply_node(L++, v);
      if (R & 1) apply_node(--R, v);
    }

    for (int i = cl + 1; i <= w; ++i) push_up(l >> i);
    for (int i = cr + 1; i <= w; ++i) push_up(r >> i);
    for (l >>= w + 1; l > 0; l >>= 1) push_up(l);
  }

  Info prod(int l, int r) {
    CHECK(0 <= l && l <= r && r <= size);
    if (l == r) return T::id();
    l += n, r += n;
    int w = my_bit::bit_width(l ^ r);
    int cl = my_bit::countr_zero(l);
    int cr = my_bit::countr_zero(r);

    for (int i = h; i > cl; --i) push_down(l >> i);
    for (int i = std::max(cl, w); i > cr; --i) push_down(r >> i);

    Info left = T::id(), right = T::id();
    for (; l < r; l >>= 1, r >>= 1) {
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