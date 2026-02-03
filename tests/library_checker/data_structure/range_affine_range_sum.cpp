// https://judge.yosupo.jp/problem/range_affine_range_sum

#pragma GCC optimize(3)
#include <bits/stdc++.h>
#include "lib/utils/fast_io.hpp"
#include "lib/ds/seg_tree/lazy_seg_tree.hpp"
using namespace std;

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 20> io;

constexpr int P = 998244353;

inline constexpr uint32_t reduce(uint32_t x) {
  return std::min(x, x - P);
}

struct RangeAffineRangeSum {
  struct Info {
    uint32_t sum, len;
  };

  struct Tag {
    uint32_t b, c;
  };

  static constexpr Info id() noexcept { return {0, 0}; };
  static constexpr Tag tag_id() noexcept { return {1, 0}; };

  static constexpr Info op(const Info& lhs, const Info& rhs) {
    return {reduce(lhs.sum + rhs.sum), lhs.len + rhs.len};
  }

  static constexpr void compose(Tag& lhs, const Tag& rhs) {
    lhs.b = static_cast<uint64_t>(lhs.b) * rhs.b % P;
    lhs.c = (static_cast<uint64_t>(lhs.c) * rhs.b + rhs.c) % P;
  }

  static constexpr void apply(Info& info, const Tag& tag) {
    info.sum = (static_cast<uint64_t>(tag.b) * info.sum +
                static_cast<uint64_t>(tag.c) * info.len) % P;
  }
};

void solve_main() {
  int n, q;
  io >> n >> q;

  LazySegTree<RangeAffineRangeSum> t(n, [&](int i) {
    RangeAffineRangeSum::Info info;
    io >> info.sum;
    info.len = 1;
    return info;
  });

  while (q--) {
    bool op;
    uint32_t l, r;
    io >> op >> l >> r;
    if (op == 0) {
      uint32_t b, c;
      io >> b >> c;
      t.apply(l, r, {b, c});
    } else {
      io << t.prod(l, r).sum << '\n';
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