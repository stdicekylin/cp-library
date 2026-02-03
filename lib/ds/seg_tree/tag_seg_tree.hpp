#pragma once

#include "lib/debug.hpp"
#include "lib/math/my_bit.hpp"

template <typename T>
struct TagSegTree {
  using Info = typename T::Info;
  using Tag  = typename T::Tag;

  int n = 0;
  int m = 0;
  int h = 0;
  std::vector<Info> val;
  std::vector<Tag> tag;
  std::vector<uint8_t> has_tag;

  TagSegTree() = default;
  explicit TagSegTree(int _m) { build(_m); }

  TagSegTree(int _m, auto&& func) { build(_m, func); }

  template <typename It>
  TagSegTree(It first, It last) { build(first, last); }

  void build(int _m) {
    m = _m;
    CHECK(m >= 0);
    n = 1;
    while (n < m) n <<= 1;
    h = internal::__lg(n);
    val.assign(n, T::id());
    tag.assign(n << 1, T::tag_id());
    has_tag.assign(n << 1, 0);
  }

  void build(int _m, auto&& func) {
    m = _m;
    CHECK(m >= 0);
    n = 1;
    while (n < m) n <<= 1;
    h = internal::__lg(n);
    val.assign(n, T::id());
    tag.assign(n << 1, T::tag_id());
    has_tag.assign(n << 1, 0);
    for (int i = 0; i < m; ++i) val[i] = func(i);
  }

  template <typename It>
  void build(It first, It last) {
    m = std::distance(first, last);
    CHECK(m >= 0);
    n = 1;
    while (n < m) n <<= 1;
    h = internal::__lg(n);
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
    CHECK(0 <= x && x < m);
    x += n;
    for (int i = h; i > 0; --i) push_down(x >> i);
    tag[x] = T::tag_id();
    val[x - n] = v;
  }

  Info get(int x) const {
    CHECK(0 <= x && x < m);
    Info res = val[x];
    x += n;
    for (int i = 0; i <= h; ++i) T::apply(res, tag[x >> i]);
    return res;
  }

  Info& get_ref(int x) {
    CHECK(0 <= x && x < m);
    x += n;
    for (int i = h; i > 0; --i) push_down(x >> i);
    T::apply(val[x - n], tag[x]);
    tag[x] = T::tag_id();
    return val[x - n];
  }

  void apply(int l, int r, const Tag& v) {
    CHECK(0 <= l && l <= r && r <= m);
    if (l == r) return;
    l += n, r += n;
    int w = internal::__lg(l ^ r);
    int cl = internal::countr_zero(l);
    int cr = internal::countr_zero(r);

    for (int i = h; i > cl; --i) push_down(l >> i);
    for (int i = std::max(cl, w); i > cr; --i) push_down(r >> i);

    w = internal::__lg(--l ^ r);

    cl = l;
    l = ~l & ((1 << w) - 1);
    while (l > 0) {
      int i = internal::countr_zero(l);
      l ^= 1 << i;
      apply_node(cl >> i ^ 1, v);
    }

    cr = r;
    r &= (1 << w) - 1;
    while (r > 0) {
      int i = internal::__lg(r);
      r ^= 1 << i;
      apply_node(cr >> i ^ 1, v);
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