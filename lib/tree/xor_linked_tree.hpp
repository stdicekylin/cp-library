#pragma once

#include "lib/utils/debug.hpp"
#include "lib/utils/my_type_traits.hpp"

// Based on https://codeforces.com/blog/entry/135239
template <bool weighted = false, internal::integral T = int>
struct XorLinkedTree {
  int n = 0;
  std::vector<int> deg;
  std::vector<int> link;
  std::vector<T> val;

  XorLinkedTree() = default;
  explicit XorLinkedTree(int _n) { init(_n); };

  void init(int _n) {
    n = _n;
    CHECK(n >= 0);
    deg.assign(n, 0);
    link.assign(n, 0);
    if constexpr (weighted) val.assign(n, 0);
  }

  void add_edge(int u, int v) requires(!weighted) {
    CHECK(0 <= u && u < n);
    CHECK(0 <= v && v < n);
    CHECK(u != v);
    ++deg[u], link[u] ^= v;
    ++deg[v], link[v] ^= u;
  }

  void add_edge(int u, int v, T w) requires(weighted) {
    CHECK(0 <= u && u < n);
    CHECK(0 <= v && v < n);
    CHECK(u != v);
    ++deg[u], link[u] ^= v, val[u] ^= w;
    ++deg[v], link[v] ^= u, val[v] ^= w;
  }

  template <typename F>
  void build(int root, F&& func) {
    CHECK(0 <= root && root < n);
    ++deg[root];

    for (int i = 0; i < n; ++i) {
      int u = i;
      while (deg[u] == 1) {
        int v = link[u];
        if constexpr (weighted) {
          T w = val[u];
          func(u, v, w);
          val[v] ^= w;
        } else {
          func(u, v);
        }
        deg[u] = 0;
        --deg[v], link[v] ^= u;
        u = v;
      }
    }
  }
};