#pragma once

template <uint32_t B = 24>
class IntegerSet {
 public:
  IntegerSet() : n(1 << B) {};
  explicit IntegerSet(uint32_t _n) : n(_n) {
    CHECK(0 <= n && n <= (1 << B));
  }

  bool insert(uint32_t pos) {
    CHECK(0 <= pos && pos < n);
    return insert0(pos);
  }

  bool erase(uint32_t pos) {
    CHECK(0 <= pos && pos < n);
    return erase0(pos);
  }
  
  int prev(uint32_t pos) const {
    CHECK(0 <= pos && pos < n);
    return prev0(pos);
  }

  int next(uint32_t pos) const {
    CHECK(0 <= pos && pos < n);
    return next0(pos);
  }
  
  bool get(uint32_t pos) const {
    CHECK(0 <= pos && pos < n);
    return b0[pos >> 6] >> (pos & 63) & 1;
  }

  size_t size() const { return counter; }

 private:
  static constexpr uint32_t L0 = B >= 6  ? B - 6  : 0;
  static constexpr uint32_t L1 = B >= 12 ? B - 12 : 0;
  static constexpr uint32_t L2 = B >= 18 ? B - 18 : 0;

  uint32_t n = 0;
  size_t counter = 0;
  uint64_t b0[1 << L0]{};
  uint64_t b1[1 << L1]{};
  uint64_t b2[1 << L2]{};
  uint64_t b3 = 0;

  static constexpr int msb(uint64_t x) {
    return x ? 63 ^ __builtin_clzll(x) : -1;
  }

  static constexpr int lsb(uint64_t x) {
    return x ? __builtin_ctzll(x) : -1;
  }

  static constexpr int msb(uint64_t x, uint32_t pos) {
    return msb(x & ((1ull << pos) - 1));
  }

  static constexpr int lsb(uint64_t x, uint32_t pos) {
    return ++pos != 64 ? lsb(x >> pos << pos) : -1;
  }

  bool insert0(uint32_t pos) {
    uint32_t u = pos >> 6, v = pos & 63;
    if (b0[u] >> v & 1) return false;
    if (!b0[u]) insert1(u);
    b0[u] |= 1ull << v;
    ++counter;
    return true;
  }

  void insert1(uint32_t pos) {
    uint32_t u = pos >> 6, v = pos & 63;
    if (!b1[u]) insert2(u);
    b1[u] |= 1ull << v;
  }

  void insert2(uint32_t pos) {
    uint32_t u = pos >> 6, v = pos & 63;
    if (!b2[u]) b3 |= 1ull << u;
    b2[u] |= 1ull << v;
  }

  bool erase0(uint32_t pos) {
    uint32_t u = pos >> 6, v = pos & 63;
    if (!(b0[u] >> v & 1)) return false;
    b0[u] ^= 1ull << v;
    if (!b0[u]) erase1(u);
    --counter;
    return true;
  }

  void erase1(uint32_t pos) {
    uint32_t u = pos >> 6, v = pos & 63;
    b1[u] ^= 1ull << v;
    if (!b1[u]) erase2(u);
  }

  void erase2(uint32_t pos) {
    uint32_t u = pos >> 6, v = pos & 63;
    b2[u] ^= 1ull << v;
    if (!b2[u]) b3 ^= 1ull << u;
  }

  int prev0(uint32_t pos) const {
    uint32_t u = pos >> 6, v = pos & 63;
    if (b0[u]) {
      int k = msb(b0[u], v);
      if (k != -1) return u << 6 | k;
    }
    int k = prev1(u);
    return k != -1 ? k << 6 | msb(b0[k]) : -1;
  }

  int prev1(uint32_t pos) const {
    uint32_t u = pos >> 6, v = pos & 63;
    if (b1[u]) {
      int k = msb(b1[u], v);
      if (k != -1) return u << 6 | k;
    }
    int k = prev2(u);
    return k != -1 ? k << 6 | msb(b1[k]) : -1;
  }

  int prev2(uint32_t pos) const {
    uint32_t u = pos >> 6, v = pos & 63;
    if (b2[u]) {
      int k = msb(b2[u], v);
      if (k != -1) return u << 6 | k;
    }
    int k = b3 ? msb(b3, u) : -1;
    return k != -1 ? k << 6 | msb(b2[k]) : -1;
  }

  int next0(uint32_t pos) const {
    uint32_t u = pos >> 6, v = pos & 63;
    if (b0[u]) {
      int k = lsb(b0[u], v);
      if (k != -1) return u << 6 | k;
    }
    int k = next1(u);
    return k != -1 ? k << 6 | lsb(b0[k]) : -1;
  }

  int next1(uint32_t pos) const {
    uint32_t u = pos >> 6, v = pos & 63;
    if (b1[u]) {
      int k = lsb(b1[u], v);
      if (k != -1) return u << 6 | k;
    }
    int k = next2(u);
    return k != -1 ? k << 6 | lsb(b1[k]) : -1;
  }

  int next2(uint32_t pos) const {
    uint32_t u = pos >> 6, v = pos & 63;
    if (b2[u]) {
      int k = lsb(b2[u], v);
      if (k != -1) return u << 6 | k;
    }
    int k = b3 ? lsb(b3, u) : -1;
    return k != -1 ? k << 6 | lsb(b2[k]) : -1;
  }
};