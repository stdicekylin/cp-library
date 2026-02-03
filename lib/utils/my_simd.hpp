#pragma once

#pragma GCC target("avx2")
#include <immintrin.h>

namespace my_simd {

template <typename T, int N>
struct Vec {
  using value_type = T;
  using type = T;
  static constexpr int lanes = 1;
};

#define USE_SIMD(T, N)                                   \
  template <>                                            \
  struct Vec<T, N / sizeof(T)> {                         \
    using value_type = T;                                \
    using type = __attribute__((vector_size(N))) T;      \
    static constexpr int lanes = N / sizeof(T);          \
  }

USE_SIMD(int8_t, 32);
USE_SIMD(uint8_t, 32);
USE_SIMD(int16_t, 32);
USE_SIMD(uint16_t, 32);
USE_SIMD(int32_t, 32);
USE_SIMD(uint32_t, 32);
USE_SIMD(int64_t, 32);
USE_SIMD(uint64_t, 32);

USE_SIMD(float, 32);
USE_SIMD(double, 32);

USE_SIMD(int8_t, 64);
USE_SIMD(uint8_t, 64);
USE_SIMD(int16_t, 64);
USE_SIMD(uint16_t, 64);
USE_SIMD(int32_t, 64);
USE_SIMD(uint32_t, 64);
USE_SIMD(int64_t, 64);
USE_SIMD(uint64_t, 64);

USE_SIMD(float, 64);
USE_SIMD(double, 64);

#undef USE_SIMD

template <typename Vec, typename It>
void vector_load(Vec& v, It ptr) {
  memcpy(&v, ptr, sizeof(v));
}

template<typename It, typename Vec>
void vector_store(It ptr, const Vec& v) {
  memcpy(ptr, &v, sizeof(v));
}

template <typename T>
void for_each(typename T::type& v, auto&& func) {
  typename T::value_type a[T::lanes];
  vector_store(a, v);
#pragma GCC unroll 4
  for (int i = 0; i < T::lanes; ++i) {
    func(a[i], i);
  }
  vector_load(v, a);
}

}  // namespace my_simd