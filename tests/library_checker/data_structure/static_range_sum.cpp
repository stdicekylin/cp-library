// https://judge.yosupo.jp/problem/static_range_sum

#pragma GCC optimize(3)
#include <bits/stdc++.h>
#include "lib/utils/fast_io.hpp"
using namespace std;

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 20> io;

constexpr int N = 5e5 + 5;

uint64_t sum[N];

void solve_main() {
  int n, q;
  io >> n >> q;

  uint32_t v;
  for (int i = 1; i <= n; ++i) {
    io >> v;
    sum[i] = v;
  }

  partial_sum(sum + 1, sum + n + 1, sum + 1);

  uint32_t l, r;
  while (q--) {
    io.in->skip_space();
    io.in->read<uint32_t, 0>(l);
    io.in->read<uint32_t, 0>(r);
    io << sum[r] - sum[l] << '\n';
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