#pragma once

template <typename K, typename V, uint32_t N, typename Hasher, typename Mixer>
struct HashMapCore {
  static constexpr uint32_t Size  = 1u << N;
  static constexpr uint32_t Mask  = Size - 1;
  static constexpr uint32_t Limit = Size >> 7;
  static constexpr uint32_t Shift = 64 - N;

  size_t counter = 0;
  K key[Size];
  V val[Size];
  std::bitset<Size> used;
  std::vector<uint32_t> bucket;
  Hasher hasher;
  Mixer mixer;

  uint32_t get_hash(const K& k) const {
    uint64_t raw = hasher(k);
    uint64_t mix = mixer(raw);
    return mix >> Shift;
  }

  HashMapCore() { clear(); }

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

  V& operator[](const K& k) {
    uint32_t h = get_hash(k) & Mask;
    while (true) {
      if (!used[h]) {
        used[h] = 1;
        key[h] = k;
        if (++counter <= Limit) {
          bucket.push_back(h);
        }
        return val[h] = V{};
      }
      if (key[h] == k) {
        return val[h];
      }
      h = (h + 1) & Mask;
    }
  }

  bool count(const K& k) const {
    uint32_t h = get_hash(k) & Mask;
    while (true) {
      if (!used[h]) return false;
      if (key[h] == k) return true;
      h = (h + 1) & Mask;
    }
  }

  size_t size() const { return counter; }
};