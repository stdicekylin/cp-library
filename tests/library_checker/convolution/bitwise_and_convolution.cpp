// https://judge.yosupo.jp/problem/bitwise_and_convolution

#pragma GCC optimize(3)
#include <bits/stdc++.h>
#include "lib/utils/fast_io.hpp"
#include "lib/math/set_func/fwt_and_simd.hpp"
using namespace std;

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 20> io;

constexpr int N = 2e5 + 5;
constexpr uint32_t P = 998244353;

void solve_main() {
  int n, m;
  io >> n;
  m = 1 << n;

  vector<uint32_t> a(m), b(m);
  for (auto& x : a) io >> x;
  for (auto& x : b) io >> x;

  my_simd::fwt_and<P>(a.data(), m);
  my_simd::fwt_and<P>(b.data(), m);
  for (int i = 0; i < m; ++i) {
    a[i] = static_cast<uint64_t>(a[i]) * b[i] % P;
  }
  my_simd::ifwt_and<P>(a.data(), m);

  for (auto& x : a) {
    io << x << ' ';
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