// https://judge.yosupo.jp/problem/point_set_range_composite_large_array

#include <bits/stdc++.h>
#include "lib/utils/fast_io.hpp"
#include "lib/ds/seg_tree/seg_tree.hpp"
#include "lib/algo/radix_sort.hpp"
using namespace std;

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 20> io;

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

  vector<tuple<bool, uint32_t, uint32_t, uint32_t>> qry(q);
  vector<uint64_t> aux;
  aux.reserve(q << 1);

  for (int i = 0; i < q; ++i) {
    auto& [op, a, b, c] = qry[i];
    io >> op >> a >> b >> c;
    if (!op) {
      aux.push_back(static_cast<uint64_t>(i << 2) << 32 | a);
    } else {
      aux.push_back(static_cast<uint64_t>(i << 2 | 1) << 32 | a);
      aux.push_back(static_cast<uint64_t>(i << 2 | 2) << 32 | b);
    }
  }

  radix_sort_u32<uint64_t, 15>(aux.data(), aux.size(), [&](auto x) {
    return static_cast<uint32_t>(x);
  });

  uint32_t lst = -1u;
  int m = 0;
  for (auto val : aux) {
    uint32_t x = static_cast<uint32_t>(val);
    uint32_t y = val >> 32;
    auto& t = qry[y >> 2];
    if (y & 3) {
      (y & 1 ? get<1>(t) : get<2>(t)) = m - (x == lst);
    } else {
      m += (x != lst);
      lst = x;
      get<1>(t) = m - 1;
    }
  }

  SegTree<PointSetRangeComposite> t(m);

  auto eval = [&](uint32_t x, const auto& info) -> uint32_t {
    return (static_cast<uint64_t>(x) * info.first + info.second) % P;
  };

  for (const auto& [op, a, b, c] : qry) {
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