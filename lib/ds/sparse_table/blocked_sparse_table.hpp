#pragma once

#include "lib/utils/debug.hpp"
#include "lib/math/my_bit.hpp"

template <typename T, uint32_t B>
struct BlockedSparseTable {
  using Info = typename T::Info;

  int n = 0;
  int m = 0;
  int size = 0;
  int h = 0;
  std::vector<std::vector<Info>> st;
  std::vector<Info> val, pre, suf;

  BlockedSparseTable() = default;

  template <typename It>
  BlockedSparseTable(It first, It last) { build(first, last); }

  template <typename F>
  BlockedSparseTable(int _n, F&& func) { build(_n, func); }

  template <typename It>
  void build(It first, It last) {
    n = std::distance(first, last);
    CHECK(n >= 0);
    if (n == 0) {
      h = 0;
      st.clear();
      return;
    }

    size = (n + B - 1) / B;
    m = size * B;
    h = internal::__lg(size) + 1;
    st.resize(h);
    st[0].resize(size);
    val.assign(m, T::id());
    pre.assign(m, T::id());
    suf.assign(m, T::id());

    for (int i = 0; i < n; ++i) {
      val[i] = *first++;
      pre[i] = i % B == 0 ? val[i] : T::op(pre[i - 1], val[i]);
    }

    for (int i = 0; i < m; i += B) {
      int down = i, up = i + B - 1;
      suf[up] = val[up];
      while (--up >= down) {
        suf[up] = T::op(val[up], suf[up + 1]);
      }
      st[0][i / B] = suf[down];
    }

    for (int i = 1; i < h; ++i) {
      int len = 1 << i;
      int half = len >> 1;
      st[i].resize(size - len + 1);
      for (int j = 0; j < size - len + 1; ++j) {
        st[i][j] = T::op(st[i - 1][j], st[i - 1][j + half]);
      } 
    }
  }

  template <typename F>
  void build(int _n, F&& func) {
    CHECK(_n >= 0);
    std::vector<Info> vec(_n);
    for (int i = 0; i < _n; ++i) {
      vec[i] = func(i);
    }
    build(vec.begin(), vec.end());
  }

  Info prod(int l, int r) const {
    CHECK(0 <= l && l <= r && r <= n);
    if (l == r) [[unlikely]] return T::id();
    --r;
    int L = l / B, R = r / B;
    if (L == R) {
      Info res = val[l];
      for (int i = l + 1; i <= r; ++i) {
        res = T::op(res, val[i]);
      }
      return res;
    } else if (L == R - 1) {
      return T::op(suf[l], pre[r]);
    } else {
      ++L;
      int k = internal::__lg(R - L);
      return T::op(T::op(suf[l], st[k][L]), T::op(st[k][R - (1 << k)], pre[r]));
    }
  }
};