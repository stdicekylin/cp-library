// https://judge.yosupo.jp/problem/unionfind

#include <bits/stdc++.h>
#include "lib/misc/fast_io.hpp"
#include "lib/ds/dsu/dsu.hpp"
using namespace std;

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 20> io;

constexpr int N = 2e5 + 5;

void solve_main() {
  int n, q;
  io >> n >> q;

  DSU dsu(n);

  while (q--) {
    bool op;
    uint32_t u, v;
    io >> op >> u >> v;
    if (op == 0) {
      dsu.unite(u, v);
    } else {
      io << dsu.same(u, v) << '\n';
    }
  }
}

int main() {
#ifdef LOCAL
  assert(freopen("test.in", "r", stdin));
  assert(freopen("test.out", "w", stdout));
#endif
  // cin.tie(nullptr)->sync_with_stdio(false);
  io.init();

  int T;
  // cin >> T;
  // io >> T;
  T = 1;

  while (T--) {
    solve_main();
  }

  return 0;
}