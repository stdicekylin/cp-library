#pragma once

template <typename K, typename V, uint32_t N>
struct FastHashMap {
  static constexpr uint32_t Size  = 1u << N;
  static constexpr uint32_t Mask  = Size - 1;
  static constexpr uint32_t Shift = 64 - N;
  static constexpr uint64_t Magic = 11995408973635179863ull;

  std::array<K, Size> key;
  std::array<V, Size> val;
  std::bitset<Size> used;

  static constexpr uint32_t get_hash(const K& k) {
    return (k * Magic >> Shift) & Mask;
  }

  FastHashMap() { clear(); }

  void clear() { used.reset(); }

  V& operator[](const K& k) {
    uint32_t h = get_hash(k);
    while (true) {
      if (!used[h]) {
        used[h] = 1;
        key[h] = k;
        return val[h] = V{};
      }
      if (key[h] == k) return val[h];
      ++h &= Mask;
    }
  }

  V get(const K& k) const {
    uint32_t h = get_hash(k);
    while (true) {
      if (!used[h]) return V{};
      if (key[h] == k) return val[h];
      ++h &= Mask;
    }
  }

  bool count(const K& k) const {
    uint32_t h = get_hash(k);
    while (true) {
      if (!used[h]) return false;
      if (key[h] == k) return true;
      ++h &= Mask;
    }
  }
};