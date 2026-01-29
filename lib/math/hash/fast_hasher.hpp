#pragma once

template <typename T>
struct FastHasher {
  static constexpr uint64_t Magic = 11995408973635179863ull;
  uint64_t operator()(T raw) const {
    return Magic * static_cast<uint64_t>(raw);
  }
};