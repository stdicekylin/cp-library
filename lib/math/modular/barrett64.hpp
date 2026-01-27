#pragma once

#include "lib/debug.hpp"

struct Barrett64 {
  uint64_t mod, inv;

  Barrett64() = default;
  explicit Barrett64(uint64_t m) {
    set_mod(m);
  }

  constexpr void set_mod(uint64_t m) {
    CHECK(m >= 2);
    mod = m;
    inv = -1ull / m + 1;
  }

  constexpr uint64_t reduce(uint64_t x) const {
    x -= static_cast<uint64_t>(static_cast<__uint128_t>(x) * inv >> 64) * mod;
    x += (x >> 63) * mod;
    return x;
  }
};

using Barrett = Barrett64;