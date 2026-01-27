// https://judge.yosupo.jp/problem/predecessor_problem

#include <bits/stdc++.h>
#include "lib/misc/fast_io.hpp"
#include "lib/ds/set/integer_set.hpp"
using namespace std;

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 21, 8> io;

constexpr int N = 2e5 + 5;

int n, q;

IntegerSet<24> s;
string t;

void solve_main() {
  io >> n >> q;
  io >> t;

  for (int i = 0; i < n; ++i) {
    if (t[i] == '1') {
      s.insert(i);
    }
  }

  while (q--) {
    int op, k;
    io >> op >> k;
    if (op == 0) {
      s.insert(k);
    } else if (op == 1) {
      s.erase(k);
    } else if (op == 2) {
      io << s.get(k) << '\n';
    } else if (op == 3) {
      if (k != 0) {
        io << s.next(k - 1) << '\n';
      } else {
        io << (s.get(0) ? 0 : s.next(0)) << '\n';
      }
    } else {
      io << s.prev(k + 1) << '\n';
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