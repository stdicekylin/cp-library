#include <bits/stdc++.h>
#include "lib/utils/timer.hpp"
#include "lib/ds/seg_tree/lazy_seg_tree.hpp"
using namespace std;

using ll  = long long;
using ull = unsigned long long;

mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());

struct RangeAddRangeSum {
  struct Info {
    ll sum;
    int len;
  };

  struct Tag {
    ll add;
  };

  static constexpr Info id() noexcept { return Info{0, 0}; }
  static constexpr Tag tag_id() noexcept { return Tag{0}; }

  static constexpr Info op(const Info& a, const Info& b) {
    return Info{a.sum + b.sum, a.len + b.len};
  }

  static constexpr void apply(Info& a, const Tag& t) {
    a.sum += t.add * a.len;
  }

  static constexpr void compose(Tag& tag, const Tag& other) {
    tag.add += other.add;
  }
};

void test_main() {
  ScopeTimer timer("test_lazy_seg_tree");

  constexpr int n = 3000;
  constexpr int V = 100000;

  vector<ll> a(n);
  for (int i = 0; i < n; ++i) a[i] = rnd() % V;

  vector<RangeAddRangeSum::Info> init(n);
  for (int i = 0; i < n; ++i) init[i] = RangeAddRangeSum::Info{a[i], 1};
  LazySegTree<RangeAddRangeSum> seg(init.begin(), init.end());

  auto naive_sum = [&](int l, int r) -> ll {
    ll s = 0;
    for (int i = l; i < r; ++i) s += a[i];
    return s;
  };

  auto naive_add = [&](int l, int r, int v) -> void {
    for (int i = l; i < r; ++i) a[i] += v;
  };

  for (int tc = 0; tc < 200000; ++tc) {
    int opt = rnd() % 6;
    int l = static_cast<int>(rnd() % (n + 1));
    int r = static_cast<int>(rnd() % (n + 1));
    if (l > r) swap(l, r);

    if (opt <= 3) {
      ll v = static_cast<int>(rnd() % (V << 1)) - V;
      seg.apply(l, r, RangeAddRangeSum::Tag{v});
      naive_add(l, r, v);
    } else {
      ll got = seg.prod(l, r).sum;
      ll exp = naive_sum(l, r);
      if (got != exp) {
        cerr << "[FAIL] lazy_seg_tree tc = " << tc << "\n"
             << "l = " << l << ", r = " << r << "\n"
             << "got = " << got << ", exp = " << exp << "\n";
        return;
      }
    }
  }

  if (true) {
    ll got = seg.prod(0, n).sum;
    ll exp = naive_sum(0, n);
    if (got != exp) {
      cerr << "[FAIL] lazy_seg_tree full range\n";
      cerr << "got = " << got << ", exp = " << exp << "\n";
      return;
    }
  }

  cerr << "[OK] lazy_seg_tree passed.\n";
}

int main() {
  int T;
  // cin >> T;
  T = 1;

  while (T--) {
    test_main();
  }
  return 0;
}
