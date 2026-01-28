// https://judge.yosupo.jp/problem/many_aplusb

#include <bits/stdc++.h>
#include "lib/misc/fast_io.hpp"
using namespace std;

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 21> io;

constexpr int N = 2e5 + 5;

void solve_main() {
  uint64_t a, b;
  io >> a >> b;
  io << a + b << '\n';
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
  io >> T;
  // T = 1;

  while (T--) {
    solve_main();
  }

  return 0;
}