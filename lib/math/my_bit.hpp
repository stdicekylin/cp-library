#pragma once

namespace my_bit {

template <typename T>
constexpr int bit_width(T x) noexcept {
  return sizeof(T) <= 4 ? 31 ^ __builtin_clz(static_cast<uint32_t>(x))
                        : 63 ^ __builtin_clzll(static_cast<uint64_t>(x));
}

template <typename T>
constexpr int countl_zero(T x) noexcept {
  return sizeof(T) <= 4 ? __builtin_clz(static_cast<uint32_t>(x))
                        : __builtin_clzll(static_cast<uint64_t>(x));
}

template <typename T>
constexpr int countr_zero(T x) noexcept {
  return sizeof(T) <= 4 ? __builtin_ctz(static_cast<uint32_t>(x))
                        : __builtin_ctzll(static_cast<uint64_t>(x));
}

template <typename T>
constexpr int popcount(T x) noexcept {
  return sizeof(T) <= 4 ? __builtin_popcount(static_cast<uint32_t>(x))
                        : __builtin_popcountll(static_cast<uint64_t>(x));
}

}