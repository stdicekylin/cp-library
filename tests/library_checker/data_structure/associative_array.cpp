// https://judge.yosupo.jp/problem/associative_array

#include <bits/stdc++.h>
#include "lib/misc/fast_io.hpp"
#include "lib/ds/hash_map/fast_hash_map.hpp"
using namespace std;

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 22> io;

constexpr int N = 2e5 + 5;

FastHashMap<uint64_t, uint64_t, 20> mp;

void solve_main() {
  int q;
  io >> q;

  while (q--) {
    bool op;
    uint64_t k;
    io >> op >> k;
    if (op == 0) {
      uint64_t v;
      io >> v;
      mp[k] = v;
    } else {
      io << mp[k] << '\n';
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