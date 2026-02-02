#pragma once

#include "lib/debug.hpp"
#include "lib/utils/my_simd.hpp"
#include "lib/math/my_bit.hpp"

namespace my_simd {

template <uint32_t P, int N, typename It>
std::enable_if_t<(N <= 1), void> fwt_or(It f) {}

template <uint32_t P, int N, typename It>
std::enable_if_t<(N > 1), void> fwt_or(It f) {
  auto reduce = [&](uint32_t x) -> uint32_t {
    return std::min(x, x - P);
  };

  static constexpr int len = N >> 1;

  fwt_or<P, len>(f);
  fwt_or<P, len>(f + len);

  for (int i = 0; i < len; ++i) {
    f[i + len] = reduce(f[i] + f[i + len]);
  }
}

template <uint32_t P, int N, typename It>
std::enable_if_t<(N <= 1), void> ifwt_or(It f) {}

template <uint32_t P, int N, typename It>
std::enable_if_t<(N > 1), void> ifwt_or(It f) {
  auto reduce = [&](uint32_t x) -> uint32_t {
    return std::min(x, x - P);
  };

  static constexpr int len = N >> 1;

  ifwt_or<P, len>(f);
  ifwt_or<P, len>(f + len);

  for (int i = 0; i < len; ++i) {
    f[i + len] = reduce(f[i + len] + P - f[i]);
  }
}

template <uint32_t P, typename It>
void fwt_or(It f, int n) {
  my_bit::bit_width_const<30>(n, [&]<uint32_t N>() {
    fwt_or<P, 1 << N>(f);
  });
}

template <uint32_t P, typename It>
void ifwt_or(It f, int n) {
  my_bit::bit_width_const<30>(n, [&]<uint32_t N>() {
    ifwt_or<P, 1 << N>(f);
  });
}

}  // namespace my_simd