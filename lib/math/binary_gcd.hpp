#pragma once

#include "lib/math/my_bit.hpp"

// Based on https://en.algorithmica.org/hpc/algorithms/gcd/
template <std::signed_integral T>
T binary_gcd(T a, T b) {
  CHECK(a >= 0 && b >= 0);
  if (a == 0 || b == 0) return a + b;

  int az = internal::countr_zero(a);
  int bz = internal::countr_zero(b);
  int shift = std::min(az, bz);
  b >>= bz;

  while (a != 0) {
    a >>= az;
    T diff = b - a;
    if (diff == 0) break;
    az = internal::countr_zero(diff);
    b = std::min(a, b);
    a = std::abs(diff);
  }

  return b << shift;
}