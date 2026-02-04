// https://judge.yosupo.jp/problem/staticrmq

#pragma GCC optimize(3)
#include <bits/stdc++.h>
#include "lib/utils/fast_io.hpp"
#include "lib/ds/sparse_table/blocked_sparse_table.hpp"
using namespace std;

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 20> io;

template <typename T>
struct RangeMin {
  using Info = T;

  static constexpr Info id() { return numeric_limits<T>::max(); }

  static constexpr Info op(const Info& lhs, const Info& rhs) {
    return min(lhs, rhs);
  }
};

void solve_main() {
  int n, q;
  io >> n >> q;

  BlockedSparseTable<RangeMin<uint32_t>, 16> t(n, [&](int i) {
    uint32_t x;
    io >> x;
    return x;
  });

  while (q--) {
    uint32_t l, r;
    io.in->read<uint32_t, 0>(l);
    io.in->read<uint32_t, 0>(r);
    io << t.prod(l, r) << '\n';
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