#include <bits/stdc++.h>
using namespace std;

#include "lib/misc/timer.hpp"
#include "lib/ds/set/integer_set.hpp"

mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());

template <int B>
void stress_one(int n, int ops) {
  ScopeTimer timer("test_integer_set: stress_one");
  
  set<int> s;

  auto naive_prev = [&](int pos) -> int {
    auto it = s.lower_bound(pos);
    if (it == s.begin()) return -1;
    --it;
    return *it;
  };

  auto naive_next = [&](int pos) -> int {
    auto it = s.upper_bound(pos);
    if (it == s.end()) return -1;
    return *it;
  };

  IntegerSet<B> st(n);

  for (int tc = 0; tc < ops; ++tc) {
    int op = rnd() % 8;
    int x = rnd() % n;

    if (op == 0) {
      bool got = st.insert(x);
      bool exp = s.insert(x).second;
      if (got != exp) {
        cerr << "[FAIL] integer_set insert tc = " << tc << "\n"
             << "B = " << B << ", n = " << n << ", x = " << x << "\n"
             << "got = " << got << ", exp = " << exp << "\n";
        return;
      }
    } else if (op == 1) {
      bool got = st.erase(x);
      bool exp = (s.erase(x) != 0);
      if (got != exp) {
        cerr << "[FAIL] integer_set erase tc = " << tc << "\n"
             << "B = " << B << ", n = " << n << ", x = " << x << "\n"
             << "got = " << got << ", exp = " << exp << "\n";
        return;
      }
    } else if (op == 2) {
      bool got = st.get(x);
      bool exp = (s.find(x) != s.end());
      if (got != exp) {
        cerr << "[FAIL] integer_set get tc = " << tc << "\n"
             << "B = " << B << ", n = " << n << ", x = " << x << "\n"
             << "got = " << got << ", exp = " << exp << "\n";
        return;
      }
    } else if (op == 3) {
      int got = st.prev(x);
      int exp = naive_prev(x);
      if (got != exp) {
        cerr << "[FAIL] integer_set prev tc = " << tc << "\n"
             << "B = " << B << ", n = " << n << ", x = " << x << "\n"
             << "got = " << got << ", exp = " << exp << "\n";
        return;
      }
    } else if (op == 4) {
      int got = st.next(x);
      int exp = naive_next(x);
      if (got != exp) {
        cerr << "[FAIL] integer_set next tc = " << tc << "\n"
             << "B = " << B << ", n = " << n << ", x = " << x << "\n"
             << "got = " << got << ", exp = " << exp << "\n";
        return;
      }
    } else if (op == 5) {
      size_t got = st.size();
      size_t exp = s.size();
      if (got != exp) {
        cerr << "[FAIL] integer_set size tc = " << tc << "\n"
             << "B = " << B << ", n = " << n << "\n"
             << "got = " << got << ", exp = " << exp << "\n";
        return;
      }
    } else if (op == 6) {
      int got = st.prev(0);
      int exp = -1;
      if (got != exp) {
        cerr << "[FAIL] integer_set prev(0)\n"
             << "B = " << B << ", n = " << n << "\n"
             << "got = " << got << ", exp = " << exp << "\n";
        return;
      }
    } else {
      int y = (rnd() & 1) ? 0 : (n - 1);
      int got = st.next(y);
      int exp = naive_next(y);
      if (got != exp) {
        cerr << "[FAIL] integer_set next(boundary)\n"
             << "B = " << B << ", n = " << n << ", y = " << y << "\n"
             << "got = " << got << ", exp = " << exp << "\n";
        return;
      }
    }
  }

  cerr << "[OK] integer_set B = " << B << ", n = " << n << " passed.\n";
}

void test_main() {
  if (true) {
    int n = 64;
    stress_one<6>(n, 200000);
  }

  if (true) {
    int n = 1 << 20;
    stress_one<20>(n, 200000);
  }

  if (true) {
    int n = rnd() % (1 << 20) + 1;
    stress_one<20>(n, 200000);
  }
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
