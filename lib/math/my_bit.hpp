#pragma once

namespace my_bit {

template <typename T>
constexpr std::enable_if_t<(sizeof(T) <= 4), int> __lg(T x) {
  return 31 ^ __builtin_clz(static_cast<uint32_t>(x));
}

template <typename T>
constexpr std::enable_if_t<(sizeof(T) > 4), int> __lg(T x) {
  return 63 ^ __builtin_clzll(static_cast<uint64_t>(x));
}

template <typename T>
constexpr std::enable_if_t<(sizeof(T) <= 4), int> countl_zero(T x) {
  return __builtin_clz(static_cast<uint32_t>(x));
}

template <typename T>
constexpr std::enable_if_t<(sizeof(T) > 4), int> countl_zero(T x) {
  return __builtin_clzll(static_cast<uint64_t>(x));
}

template <typename T>
constexpr std::enable_if_t<(sizeof(T) <= 4), int> countr_zero(T x) {
  return __builtin_ctz(static_cast<uint32_t>(x));
}

template <typename T>
constexpr std::enable_if_t<(sizeof(T) > 4), int> countr_zero(T x) {
  return __builtin_ctzll(static_cast<uint64_t>(x));
}

template <typename T>
constexpr std::enable_if_t<(sizeof(T) <= 4), int> popcount(T x) {
  return __builtin_popcount(static_cast<uint32_t>(x));
}

template <typename T>
constexpr std::enable_if_t<(sizeof(T) > 4), int> popcount(T x) {
  return __builtin_popcountll(static_cast<uint64_t>(x));
}

template <uint32_t Max, uint32_t N = 0, typename F>
std::enable_if_t<(N >= Max), void> bit_width_const(uint64_t n, F&& func) {}

template <uint32_t Max, uint32_t N = 0, typename F>
std::enable_if_t<(N < Max), void> bit_width_const(uint64_t n, F&& func) {
  if (n <= 1ull << N) {
    func.template operator()<N>();
  } else {
    bit_width_const<Max, N + 1>(n, func);
  }
}

}  // namespace my_bit