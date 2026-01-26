#pragma once

#include "lib/math/my_bit.hpp"

// Based on https://en.algorithmica.org/hpc/algorithms/gcd/
template <typename T>
T binary_gcd(T a, T b) {
  static_assert(std::is_signed<T>::value);
  CHECK(a >= 0 && b >= 0);
  if (a == 0 || b == 0) return a + b;

  int az = my_bit::countr_zero(a);
  int bz = my_bit::countr_zero(b);
  int shift = std::min(az, bz);
  b >>= bz;

  while (a != 0) {
    a >>= az;
    T diff = b - a;
    if (diff == 0) break;
    az = my_bit::countr_zero(diff);
    b = std::min(a, b);
    a = std::abs(diff);
  }

  return b << shift;
}