// https://qoj.ac/problem/21979

#include <bits/stdc++.h>
#include "lib/utils/fast_io.hpp"
#include "lib/ds/dsu/dsu.hpp"
#include "lib/algo/radix_sort.hpp"
using namespace std;

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 20> io;

void solve_main() {
  int n, m;
  io >> n >> m;

  vector<pair<uint32_t, uint32_t>> edge(m);
  vector<uint64_t> id(m);

  for (int i = 0; i < m; ++i) {
    auto& [a, b] = edge[i];
    io.in->read<uint32_t, 0>(a);
    io.in->read<uint32_t, 0>(b);
    --a, --b;
    uint32_t c;
    io.in->read<uint32_t, 1>(c);
    id[i] = static_cast<uint64_t>(i) << 32 | c;
  }

  radix_sort_u32<uint64_t, 15>(id.data(), m, [&](auto x) {
    return static_cast<uint32_t>(x);
  });

  DSU dsu(n);
  uint64_t ans = 0;
  for (auto x : id) {
    int i = x >> 32;
    if (dsu.unite(edge[i].first, edge[i].second)) {
      ans += static_cast<uint32_t>(x);
    }
  }

  io << ans << '\n';
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