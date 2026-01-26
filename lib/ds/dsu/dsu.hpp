#pragma once

struct DSU {
  int n = 0;
  std::vector<int> fa;

  DSU() = default;
  explicit DSU(int _n) { resize(_n); }

  void resize(int _n) {
    CHECK(_n >= 0);
    n = _n;
    fa.assign(n, -1);
  }

  int find(int x) {
    CHECK(0 <= x && x < n);
    return fa[x] < 0 ? x : fa[x] = find(fa[x]);
  }

  int size(int x) {
    CHECK(0 <= x && x < n);
    x = find(x);
    return -fa[x];
  }

  bool unite(int x, int y) {
    CHECK(0 <= x && x < n);
    CHECK(0 <= y && y < n);
    x = find(x);
    y = find(y);
    if (x == y) return false;
    if (fa[x] > fa[y]) std::swap(x, y);
    fa[x] += fa[y];
    fa[y] = x;
    return true;
  }

  bool same(int x, int y) {
    CHECK(0 <= x && x < n);
    CHECK(0 <= y && y < n);
    return find(x) == find(y);
  }
};
