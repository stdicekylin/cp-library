#pragma once

#include "lib/math/my_bit.hpp"

template <typename T>
struct TagSegTree {
  using Info = typename T::Info;
  using Tag  = typename T::Tag;

  int n = 0;
  int size = 0;
  int h = 0;
  std::vector<Info> val;
  std::vector<Tag> tag;
  std::vector<uint8_t> has_tag;

  TagSegTree() = default;
  explicit TagSegTree(int _n) { build(_n); }

  template <typename It>
  TagSegTree(It first, It last) { build(first, last); }

  void build(int _n) {
    CHECK(_n >= 0);
    size = _n;
    n = 1;
    while (n < size) n <<= 1;
    h = my_bit::bit_width(n);
    val.assign(n, T::id());
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
    val.assign(n, T::id());
    tag.assign(n << 1, T::tag_id());
    has_tag.assign(n << 1, 0);
    std::copy(first, last, val.begin());
  }

  void reset() {
    std::fill(val.begin(), val.end(), T::id());
    std::fill(tag.begin(), tag.end(), T::tag_id());
    std::fill(has_tag.begin(), has_tag.end(), 0);
  }

  void set(int x, const Info& v) {
    CHECK(0 <= x && x < size);
    x += n;
    for (int i = h; i > 0; --i) push_down(x >> i);
    tag[x] = T::tag_id();
    val[x - n] = v;
  }

  Info get(int x) {
    CHECK(0 <= x && x < size);
    x += n;
    for (int i = h; i > 0; --i) push_down(x >> i);
    T::apply(val[x - n], tag[x]);
    tag[x] = T::tag_id();
    return val[x - n];
  }

  Info apply(int x, const Tag& v) {
    CHECK(0 <= x && x < size);
    x += n;
    for (int i = h; i > 0; --i) push_down(x >> i);
    T::apply(val[x - n], tag[x]);
    T::apply(val[x - n], v);
    tag[x] = T::tag_id();
    return val[x - n];
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
  }

 private:
  void apply_node(int x, const Tag& v) {
    has_tag[x] = 1;
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