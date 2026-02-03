// https://judge.yosupo.jp/problem/many_aplusb_128bit

#include <bits/stdc++.h>
#include "lib/utils/fast_io.hpp"
using namespace std;

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 19> io;

void solve_main() {
  __int128_t a, b;
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