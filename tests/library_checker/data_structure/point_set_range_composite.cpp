// https://judge.yosupo.jp/problem/point_set_range_composite

#include <bits/stdc++.h>
#include "lib/misc/fast_io.hpp"
#include "lib/ds/seg_tree/seg_tree.hpp"
using namespace std;

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 21> io;

constexpr int N = 2e5 + 5;
constexpr int P = 998244353;

struct PointSetRangeComposite {
  using Info = pair<uint32_t, uint32_t>;
  static constexpr Info id() noexcept { return {1, 0}; };

  static constexpr Info op(const Info& lhs, const Info& rhs) {
    return {static_cast<uint64_t>(lhs.first) * rhs.first % P,
            (static_cast<uint64_t>(lhs.second) * rhs.first + rhs.second) % P};
  }
};

void solve_main() {
  int n, q;
  io >> n >> q;

  SegTree<PointSetRangeComposite> t(n, [&](int i) {
    uint32_t a, b;
    io >> a >> b;
    return make_pair(a, b);
  });

  auto eval = [&](uint32_t x, const auto& info) -> uint32_t {
    return (1ull * x * info.first + info.second) % P;
  };

  while (q--) {
    bool op;
    uint32_t a, b, c;
    io >> op >> a >> b >> c;
    if (op == 0) {
      t.set(a, {b, c});
    } else {
      io << t.evaluate(a, b, c, eval) << '\n';
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