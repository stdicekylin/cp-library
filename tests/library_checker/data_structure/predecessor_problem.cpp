// https://judge.yosupo.jp/problem/predecessor_problem

#include <bits/stdc++.h>
#include "lib/utils/fast_io.hpp"
#include "lib/ds/set/integer_set.hpp"
using namespace std;

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 20> io;

constexpr int N = 1e7 + 100;

IntegerSet<24> s;

void solve_main() {
  int n, q;
  io >> n >> q;

  string t;
  io >> t;

  auto ptr = t.data();

  for (int i = 0, _i = n - 63; i < _i; i += 64) {
    uint64_t mask = 0;
    for (int j = 0; j < 64; ++j) {
      mask |= static_cast<uint64_t>(*ptr++ & 1) << j;
    }
    s.b0[i >> 6] = mask;
  }

  if (true) {
    uint64_t mask = 0;
    int i = n / 64 * 64;
    for (int j = 0; j < n - i; ++j) {
      mask |= static_cast<uint64_t>(*ptr++ & 1) << j;
    }
    s.b0[i >> 6] = mask;
  }

  for (int i = 0, _i = n / 64; i < _i; i += 64) {
    uint64_t mask = 0;
    for (int j = 0; j < 64; ++j) {
      mask |= 1ull * (s.b0[i + j] != 0) << j;
    }
    s.b1[i >> 6] = mask;
  }

  for (int i = 0, _i = n / 64 / 64; i < _i; i += 64) {
    uint64_t mask = 0;
    for (int j = 0; j < 64; ++j) {
      mask |= 1ull * (s.b1[i + j] != 0) << j;
    }
    s.b2[i >> 6] = mask;
  }

  if (true) {
    s.b3 = 0;
    for (int j = 0; j < 64; ++j) {
      s.b3 |= 1ull * (s.b2[j] != 0) << j;
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