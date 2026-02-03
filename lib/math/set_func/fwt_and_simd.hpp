#pragma once

#include "lib/utils/debug.hpp"
#include "lib/utils/my_simd.hpp"
#include "lib/math/my_bit.hpp"

namespace my_simd {

template <uint32_t P, int N, typename It>
void fwt_and(It f) {
  auto add = [&](uint32_t x, uint32_t y) -> uint32_t {
    x += y;
    return std::min(x, x - P);
  };

  static constexpr int len = N >> 1;

  if constexpr (len > 0) {
    fwt_and<P, len>(f);
    fwt_and<P, len>(f + len);
    for (int i = 0; i < len; ++i) {
      f[i] = add(f[i], f[i + len]);
    }
  }
}

template <uint32_t P, int N, typename It>
void ifwt_and(It f) {
  auto sub = [&](uint32_t x, uint32_t y) -> uint32_t {
    x -= y;
    return std::min(x, x + P);
  };

  static constexpr int len = N >> 1;

  if constexpr (len > 0) {
    ifwt_and<P, len>(f);
    ifwt_and<P, len>(f + len);
    for (int i = 0; i < len; ++i) {
      f[i] = sub(f[i], f[i + len]);
    }
  }
}

template <uint32_t P, typename It>
void fwt_and(It f, int n) {
  internal::bit_width_const<30>(n, [&]<uint32_t N>() {
    fwt_and<P, 1 << N>(f);
  });
}

template <uint32_t P, typename It>
void ifwt_and(It f, int n) {
  internal::bit_width_const<30>(n, [&]<uint32_t N>() {
    ifwt_and<P, 1 << N>(f);
  });
}

}  // namespace my_simd