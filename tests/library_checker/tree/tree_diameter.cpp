// https://judge.yosupo.jp/problem/tree_diameter

#pragma GCC optimize(3)
#include <bits/stdc++.h>
#include "lib/utils/fast_io.hpp"
#include "lib/tree/xor_linked_tree.hpp"

using namespace std;

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 20> io;

void solve_main() {
  int n;
  io >> n;
  XorLinkedTree<true, int> t(n);

  for (int i = 0; i < n - 1; ++i) {
    uint32_t u, v, w;
    io.in->read<uint32_t, 0>(u);
    io.in->read<uint32_t, 0>(v);
    io >> w;
    t.add_edge(u, v, w);
  }

  vector<ll> mx(n, 0);
  vector<int> fa(n, 0);
  auto pos = views::iota(0, n) | ranges::to<vector<uint32_t>>();

  ll ans = 0;
  int l = 0, r = 0, lca = 0;

  t.build(0, [&](int u, int v, int w) -> void {
    if (u == 0) return;
    fa[u] = v;

    ll new_mx = mx[u] + w;
    ll new_ans = new_mx + mx[v];

    if (new_ans > ans) {
      ans = new_ans;
      l = pos[u], r = pos[v], lca = v;
    }

    if (new_mx > mx[v]) {
      mx[v] = new_mx;
      pos[v] = pos[u];
    }
  });

  int cnt = 0;

  while (l != lca) {
    pos[cnt] = l, ++cnt;
    l = fa[l];
  }

  pos[cnt] = lca;
  int mid = ++cnt;

  while (r != lca) {
    pos[cnt] = r, ++cnt;
    r = fa[r];
  }

  io << ans << ' ' << cnt << '\n';

  for (int i = 0; i < mid; ++i) io << pos[i] << ' ';
  for (int i = cnt - 1; i >= mid; --i) io << pos[i] << ' ';
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