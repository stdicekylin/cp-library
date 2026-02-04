// https://judge.yosupo.jp/problem/range_add_range_min

#pragma GCC optimize(3)
#include <bits/stdc++.h>
#include "lib/utils/fast_io.hpp"
#include "lib/ds/seg_tree/seg_tree.hpp"
using namespace std;

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 20> io;

struct PrefixMin {
  using Info = pair<int64_t, int64_t>;

  static constexpr int64_t inf = 1e18;
  static constexpr Info id() { return make_pair(0, inf); }

  static constexpr Info op(const Info& lhs, const Info& rhs) {
    return make_pair(lhs.first + rhs.first,
                     min(lhs.second, lhs.first + rhs.second));
  }
};

void solve_main() {
  uint32_t n, q;
  io >> n >> q;

  vector<int64_t> a(n);

  int32_t lst = 0;
  for (auto& x : a) {
    int32_t v;
    io >> v;
    x = v - lst;
    lst = v;
  }

  SegTree<PrefixMin> t(n, [&](int i) {
    return make_pair(a[i], a[i]);
  });

  while (q--) {
    bool op;
    uint32_t l, r;
    io >> op;
    io.in->read<uint32_t, 0>(l);
    io.in->read<uint32_t, 0>(r);
    if (op == 0) {
      int32_t x;
      io >> x;
      a[l] += x;
      t.set(l, make_pair(a[l], a[l]));
      if (r < n) [[likely]] {
        a[r] -= x;
        t.set(r, make_pair(a[r], a[r]));
      }
    } else {
      io << t.evaluate(0, l, 0ll, [&](int64_t a, const auto& x) {
        return a + x.first;
      }) + t.prod(l, r).second << '\n';
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