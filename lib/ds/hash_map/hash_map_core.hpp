#pragma once

template <typename K, typename V, uint32_t N, typename Hasher>
struct HashMapCore {
  static constexpr uint32_t Size  = 1u << N;
  static constexpr uint32_t Mask  = Size - 1;
  static constexpr uint32_t Limit = Size >> 7;
  static constexpr uint32_t Shift = 64 - N;

  size_t counter = 0;
  std::array<K, Size> key;
  std::array<V, Size> val;
  std::bitset<Size> used;
  std::vector<uint32_t> bucket;
  Hasher hasher;

  HashMapCore() {
    clear();
    bucket.reserve(Limit);
  }

  void clear() {
    if (counter <= Limit) {
      for (int pos : bucket) {
        used[pos] = 0;
      }
    } else {
      used.reset();
    }
    bucket.clear();
    counter = 0;
  }

  uint32_t get_hash(const K& k) const {
    return (hasher(k) >> Shift) & Mask;
  }

  V& operator[](const K& k) {
    uint32_t h = get_hash(k);
    while (true) {
      if (!used[h]) {
        used[h] = 1;
        key[h] = k;
        if (++counter <= Limit) {
          bucket.push_back(h);
        }
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

  size_t size() const { return counter; }
};