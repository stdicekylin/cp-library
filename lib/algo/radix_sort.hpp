#pragma once

#include "lib/utils/debug.hpp"

template <typename T, uint32_t N = 16, typename It, typename F>
void radix_sort_u32(It f, int n, F&& func) {
  CHECK(n >= 0);

  static constexpr uint32_t Mask = (1u << N) - 1;

  if (static_cast<uint32_t>(n) < Mask / 8) {
    std::stable_sort(f, f + n, [&](const T& x, const T& y) {
      return func(x) < func(y);
    });
    return;
  }

  std::array<uint32_t, Mask + 1> cnt1{}, cnt2{};
  for (int i = 0; i < n; ++i) {
    uint32_t val = func(f[i]);
    ++cnt1[val & Mask];
    ++cnt2[val >> N];
  }

  for (uint32_t i = 0; i < Mask; ++i) {
    cnt1[i + 1] += cnt1[i];
    cnt2[i + 1] += cnt2[i];
  }

  std::vector<T> tmp(n);
  for (int i = n - 1; i >= 0; --i) {
    tmp[--cnt1[func(f[i]) & Mask]] = std::move(f[i]);
  }
  for (int i = n - 1; i >= 0; --i) {
    f[--cnt2[func(tmp[i]) >> N]] = std::move(tmp[i]);
  }
}