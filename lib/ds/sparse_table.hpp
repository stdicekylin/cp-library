#pragma once

template <typename T>
struct SparseTable {
  using Info = typename T::Info;

  int n = 0;
  int h = 0;
  vector<vector<Info>> st;

  SparseTable() = default;

  template <typename It>
  SparseTable(It first, It last) { build(first, last); }

  template <typename F>
  SparseTable(int _n, F func) { build(_n, func); }

  template <typename It>
  void build(It first, It last) {
    n = std::distance(first, last);
    CHECK(n >= 0);
    if (n == 0) {
      h = 0;
      st.clear();
      return;
    }

    h = my_bit::bit_width(n) - 1;
    st.resize(h + 1);

    st[0].resize(n);
    for (int i = 0; i < n; ++i) {
      st[0][i] = *first++;
    }

    for (int i = 1; i <= h; ++i) {
      int len = 1 << i;
      int half = len >> 1;
      st[i].resize(n - len + 1);
      for (int j = 0; j < n - len + 1; ++j) {
        st[i][j] = T::op(st[i - 1][j], st[i - 1][j + half]);
      } 
    }
  }

  template <typename F>
  void build(int _n, F func) {
    CHECK(_n >= 0);
    std::vector<Info> vec(_n);
    for (int i = 0; i < _n; ++i) {
      vec[i] = func(i);
    }
    build(vec.begin(), vec.end());
  }

  Info prod(int l, int r) const {
    CHECK(0 <= l && l <= r && r <= n);
    if (l == r) return T::id();
    int k = my_bit::bit_width(r - l) - 1;
    return T::op(st[k][l], st[k][r - (1 << k)]);
  }
};