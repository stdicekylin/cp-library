#pragma once

// Based on https://xoshiro.di.unimi.it/splitmix64.c
struct SplitMix64 {
  uint64_t seed;

  SplitMix64()
      : seed(std::chrono::steady_clock::now().time_since_epoch().count()) {}

  uint64_t operator()(uint64_t x) const {
    x += seed + 0x9e3779b97f4a7c15ull;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ull;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebull;
    return x ^ (x >> 31);
  }
};