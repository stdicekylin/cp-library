// https://judge.yosupo.jp/problem/bitwise_xor_convolution

#pragma GCC optimize(3)
#include <bits/stdc++.h>
#include "lib/utils/fast_io.hpp"
#include "lib/math/set_func/fwt_xor_simd.hpp"
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

  vector<uint32_t> a(m), b(m), c(m);
  for (auto& x : a) io >> x;
  for (auto& x : b) io >> x;

  my_simd::fwt_xor<P>(a.data(), m);
  my_simd::fwt_xor<P>(b.data(), m);
  for (int i = 0; i < m; ++i) {
    c[i] = static_cast<uint64_t>(a[i]) * b[i] % P;
  }
  my_simd::fwt_xor<P>(c.data(), m);

  auto qpow = [&](uint64_t x, uint32_t y, uint64_t k = 1) -> uint32_t {
    for (; y; y >>= 1, x = x * x % P) {
      if (y & 1) k = k * x % P;
    }
    return k;
  };

  uint32_t inv = qpow(m, P - 2);
  for (int i = 0; i < m; ++i) {
    c[i] = static_cast<uint64_t>(c[i]) * inv % P;
    io << c[i] << ' ';
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