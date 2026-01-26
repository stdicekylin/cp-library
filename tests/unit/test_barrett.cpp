#include <bits/stdc++.h>
using namespace std;

#ifdef LOCAL
#include "lib/debug.hpp"
#else
#define CHECK(expr)
#endif

#include "lib/misc/timer.hpp"
#include "lib/math/modular/barrett.hpp"
#include "lib/math/modular/barrett32.hpp"

mt19937_64 rnd_64(chrono::steady_clock::now().time_since_epoch().count());
mt19937 rnd_32(chrono::steady_clock::now().time_since_epoch().count());

void test_main() {
  ScopeTimer timer("test_barrett");

  // 64-bit reduce test
  if (true) {
    vector<uint64_t> mods = {2ull, 3ull, 5ull, 7ull, 998244353ull,
                             1000000007ull, (1ull << 61) - 1,
                             (1ull << 62) - 57, (1ull << 63) - 25};

    for (int i = 0; i < 500; ++i) {
      uint64_t m = rnd_64();
      if (m <= 1) continue;
      mods.push_back(m);
    }

    Barrett bar;
    for (uint64_t mod : mods) {
      bar.set_mod(mod);
      for (int tc = 0; tc < 20000; ++tc) {
        uint64_t x = rnd_64();
        uint64_t got = bar.reduce(x);
        uint64_t exp = x % mod;
        if (got != exp) {
          cerr << "[FAIL] barrett64 tc = " << tc << "\n"
               << "mod = " << mod << ", x = " << x << ", "
               << "got = " << got << ", exp = " << exp << "\n";
          return;
        }
      }
    }
  }

  // 32-bit reduce test
  if (true) {
    vector<uint32_t> mods = {2u, 3u, 5u, 7u, 998244353u, 1000000007u};

    for (int i = 0; i < 500; ++i) {
      uint64_t m = rnd_32();
      if (m <= 1) continue;
      mods.push_back(m);
    }

    Barrett32 bar;
    for (uint32_t mod : mods) {
      bar.set_mod(mod);
      for (int tc = 0; tc < 20000; ++tc) {
        uint32_t x = rnd_32();
        uint32_t got = bar.reduce(x);
        uint32_t exp = x % mod;
        if (got != exp) {
          cerr << "[FAIL] barrett64 tc = " << tc << "\n"
               << "mod = " << mod << ", x = " << x << ", "
               << "got = " << got << ", exp = " << exp << "\n";
          return;
        }
      }
    }
  }

  cerr << "[OK] barrett reduce passed.\n";
}

int main() {
  int T;
  // cin >> T; 
  T = 1;

  while (T--) {
    test_main();
  }
  return 0;
}