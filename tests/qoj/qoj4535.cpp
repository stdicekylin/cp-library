// https://qoj.ac/problem/4535

#include <bits/stdc++.h>
#pragma GCC optimize(3)
#include "lib/utils/fast_io.hpp"
#include "lib/ds/seg_tree/tag_seg_tree.hpp"
using namespace std;

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 22> io;

constexpr int N = 2e5 + 5;
constexpr ll inf = 1e18;

struct Node {
  struct Info {
    ll x, hx;
  };

  struct Tag {
    ll x, y, hx, hy;
  };

  static constexpr Info id() noexcept { return {0, 0}; }
  static constexpr Tag tag_id() noexcept { return {0, -inf, 0, -inf}; }

  static constexpr void apply(Info& info, const Tag& tag) noexcept {
    info.hx = max({info.hx, info.x + tag.hx, tag.hy});
    info.x = max(info.x + tag.x, tag.y);
  }

  static constexpr void compose(Tag& tag, const Tag& other) noexcept {
    tag.hx = max(tag.hx, tag.x + other.hx);
    tag.hy = max({tag.hy, other.hy, tag.y + other.hx});
    tag.x = max(tag.x + other.x, -inf);
    tag.y = max(tag.y + other.x, other.y);
  }
};

void solve_main() {
  int n, m;
  io >> n >> m;

  TagSegTree<Node> t(n, [&](int i) {
    uint32_t v;
    io >> v;
    Node::Info info;
    info.x = info.hx = v;
    return info;
  });

  while (m--) {
    char op;
    uint32_t l, r;
    int x;
    io >> op;
    if (op == '1') {
      io.in->read<uint32_t, 0>(l);
      io.in->read<uint32_t, 0>(r);
      io >> x;
      --l;
      t.apply(l, r, {x, -inf, x, -inf});
    } else if (op == '2') {
      io.in->read<uint32_t, 0>(l);
      io.in->read<uint32_t, 0>(r);
      io >> x;
      --l;
      t.apply(l, r, {-x, 0, -x, 0});
    } else if (op == '3') {
      io.in->read<uint32_t, 0>(l);
      io.in->read<uint32_t, 0>(r);
      io >> x;
      --l;
      t.apply(l, r, {-inf, x, -inf, x});
    } else if (op == '4') {
      io.in->read<uint32_t, 0>(l);
      --l;
      io << t.get(l).x << '\n';
    } else {
      io.in->read<uint32_t, 0>(l);
      --l;
      io << t.get(l).hx << '\n';
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