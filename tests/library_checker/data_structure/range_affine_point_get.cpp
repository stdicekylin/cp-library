// https://judge.yosupo.jp/problem/range_affine_point_get

#include <bits/stdc++.h>
#include "lib/utils/fast_io.hpp"
#include "lib/ds/seg_tree/tag_seg_tree.hpp"
using namespace std;

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 21> io;

constexpr int N = 2e5 + 5;
constexpr int P = 998244353;

inline constexpr uint32_t reduce(uint32_t x) {
  return x - (x >= P) * P;
}

struct RangeAffinePointGet {
  using Info = uint32_t;

  struct Tag {
    uint32_t b, c;
  };

  static constexpr Info id() noexcept { return 0; };
  static constexpr Tag tag_id() noexcept { return {1, 0}; };

  static constexpr void compose(Tag& lhs, const Tag& rhs) {
    lhs.b = static_cast<uint64_t>(lhs.b) * rhs.b % P;
    lhs.c = (static_cast<uint64_t>(lhs.c) * rhs.b + rhs.c) % P;
  }

  static constexpr void apply(Info& info, const Tag& tag) {
    info = (static_cast<uint64_t>(tag.b) * info + tag.c) % P;
  }
};

void solve_main() {
  int n, q;
  io >> n >> q;

  TagSegTree<RangeAffinePointGet> t(n, [&](int i) {
    uint32_t v;
    io >> v;
    return v;
  });

  while (q--) {
    bool op;
    io >> op;
    if (op == 0) {
      uint32_t l, r, b, c;
      io.in->read<uint32_t, 0>(l);
      io.in->read<uint32_t, 0>(r);
      io >> b >> c;
      t.apply(l, r, {b, c});
    } else {
      uint32_t i;
      io.in->read<uint32_t, 0>(i);
      io << t.get(i) << '\n';
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