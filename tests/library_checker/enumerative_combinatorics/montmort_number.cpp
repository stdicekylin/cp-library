// https://judge.yosupo.jp/problem/montmort_number_mod

#include <bits/stdc++.h>
#include "lib/utils/fast_io.hpp"
#include "lib/math/modular/barrett64.hpp"
using namespace std;

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 20> io;

void solve_main() {
  int n, m;
  io >> n >> m;
  Barrett bar(m);

  uint32_t res = 0;

  for (int i = 2, op = 1; i <= n + 1; ++i) {
    io << res << ' ';
    res = bar.reduce(static_cast<uint64_t>(res) * i + op);
    op = -op;
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