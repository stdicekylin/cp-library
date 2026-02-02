#pragma once

#include "lib/debug.hpp"
#include "lib/utils/my_simd.hpp"
#include "lib/math/my_bit.hpp"

namespace my_simd {

template <uint32_t P, int N, typename It>
std::enable_if_t<(N <= 1), void> fwt_xor(It f) {}

template <uint32_t P, int N, typename It>
std::enable_if_t<(N > 1), void> fwt_xor(It f) {
  auto reduce = [&](uint32_t x) -> uint32_t {
    return std::min(x, x - P);
  };

  static constexpr int len = N >> 1;

  fwt_xor<P, len>(f);
  fwt_xor<P, len>(f + len);

  for (int i = 0; i < len; ++i) {
    uint32_t x = reduce(f[i] + f[i + len]);
    uint32_t y = reduce(f[i] + P - f[i + len]);
    f[i] = x, f[i + len] = y;
  }
}

template <uint32_t P, int N, typename It>
std::enable_if_t<(N <= 1), void> ifwt_xor(It f) {}

template <uint32_t P, int N, typename It>
std::enable_if_t<(N > 1), void> ifwt_xor(It f) {
  auto reduce = [&](uint32_t x) -> uint32_t {
    return std::min(x, x - P);
  };

  static constexpr int len = N >> 1;

  ifwt_xor<P, len>(f);
  ifwt_xor<P, len>(f + len);

  for (int i = 0; i < len; ++i) {
    uint32_t x = f[i] + f[i + len];
    uint32_t y = f[i] + P - f[i + len];
    f[i] = reduce((x + (x & 1) * P) >> 1);
    f[i + len] = reduce((y + (y & 1) * P) >> 1);
  }
}

template <uint32_t P, typename It>
void fwt_xor(It f, int n) {
  my_bit::bit_width_const<30>(n, [&]<uint32_t N>() {
    fwt_xor<P, 1 << N>(f);
  });
}

template <uint32_t P, typename It>
void ifwt_xor(It f, int n) {
  my_bit::bit_width_const<30>(n, [&]<uint32_t N>() {
    ifwt_xor<P, 1 << N>(f);
  });
}

}  // namespace my_simd