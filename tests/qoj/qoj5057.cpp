// https://qoj.ac/problem/5057

#pragma GCC optimize(3)
#include <bits/stdc++.h>
#include "lib/utils/fast_io.hpp"
#include "lib/ds/seg_tree/lazy_seg_tree.hpp"
using namespace std;

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 20> io;

struct Node {
  struct Info {
    uint32_t odd, even;
    uint64_t sum;
  };

  struct Tag {
    uint32_t odd, even;
    bool rev;
  };

  static constexpr Info id() { return {0, 0, 0}; }
  static constexpr Tag tag_id() { return {0, 0, 0}; }

  static constexpr Info op(const Info& lhs, const Info& rhs) {
    return {lhs.odd + rhs.odd, lhs.even + rhs.even, lhs.sum + rhs.sum};
  }

  static constexpr void apply(Info& info, const Tag& tag) {
    if (tag.rev) swap(info.odd, info.even);
    info.sum += static_cast<uint64_t>(info.odd) * tag.odd +
                static_cast<uint64_t>(info.even) * tag.even;
  }

  static constexpr void compose(Tag& lhs, const Tag& rhs) {
    if (rhs.rev) lhs.rev ^= 1, swap(lhs.odd, lhs.even);
    lhs.odd += rhs.odd;
    lhs.even += rhs.even;
  }
};

constexpr int N = 5e5 + 5;

int n, m;
uint32_t a[N], lst[N], t[N];
uint64_t ans[N];
vector<pair<int, int>> qry[N];

void solve_main() {
  io >> n;
  for (int i = 1; i <= n; ++i) {
    io.in->read<uint32_t, 0>(a[i]);
    lst[i] = exchange(t[a[i]], i);
  }
  io >> m;
  for (int i = 0; i < m; ++i) {
    uint32_t l, r;
    io.in->read<uint32_t, 0>(l);
    io.in->read<uint32_t, 0>(r);
    qry[r].emplace_back(l - 1, i);
  }

  LazySegTree<Node> t(n, [&](int i) {
    Node::Info x{0, 1, 0};
    return x;
  });

  for (int i = 1; i <= n; ++i) {
    t.apply(lst[i], i, {0, 0, true});
    t.apply_all({1, 0, false});
    for (const auto& [l, id] : qry[i]) {
      ans[id] = t.prod(l, i).sum;
    }
  }

  for (int i = 0; i < m; ++i) {
    io << ans[i] << '\n';
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