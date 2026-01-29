#pragma once

#include "lib/debug.hpp"
#include "lib/misc/my_type_traits.hpp"

#ifdef __unix__
#ifndef DISABLE_MMAP
#define ENABLE_MMAP
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#endif

namespace fast_io {

template <int BufSize = 1 << 20>
struct FastInput {
  static constexpr size_t Offset = 64;

  FILE* file;
  char* buf;
  const char* cur;
  const char* end;

  explicit FastInput(FILE* _file = stdin) : file(_file) {
#ifdef ENABLE_MMAP
    struct stat st;
    int fd = fileno(file);
    fstat(fd, &st);
    size_t map_size = static_cast<size_t>(st.st_size);
    cur = static_cast<const char*>(
        mmap(nullptr, map_size + Offset, PROT_READ, MAP_PRIVATE, fd, 0));
    end = cur + map_size;
#else
    cur = buf = new char[BufSize + Offset];
    end = buf + fread(buf, 1, BufSize, file);
    memset(const_cast<char*>(end), 0, Offset);
#endif
  }

#ifndef ENABLE_MMAP
  ~FastInput() { delete[] buf; }
#endif

#ifdef ENABLE_MMAP
  void ensure(int need) { (void)need; }
#else
  void ensure(int need) {
    int rem = end - cur;
    if (__builtin_expect(rem >= need, true)) return;
    if (rem > 0 && cur != buf) memmove(buf, cur, rem);
    cur = buf;
    end = buf + rem + fread(buf + rem, 1, BufSize - rem, file);
    memset(const_cast<char*>(end), 0, Offset);
  }
#endif

  void skip_space() {
    ensure(1);
    while (__builtin_expect(*cur < 33, false)) {
      ++cur;
      ensure(1);
      CHECK(cur < end);
    }
  }

  void read(bool& x) {
    CHECK(*cur == '0' || *cur == '1');
    x = (*cur++ == '1');
    ensure(1);
    CHECK(*cur <= 32);
    ++cur;
  }

  template <typename T>
  std::enable_if_t<my_type_traits::is_unsigned_v<T>, void> read(T& x) {
    ensure(40);
    CHECK(*cur >= '0' && *cur <= '9');
    x = *cur++ & 15;

    static constexpr int count8 = std::numeric_limits<T>::digits10 / 8;

    for (int i = 0; i < count8; ++i) {
      uint64_t v;
      memcpy(&v, cur, 8);
      if ((v ^= 0x3030303030303030) & 0xf0f0f0f0f0f0f0f0) break;
      v = (v * 10 + (v >> 8)) & 0xff00ff00ff00ff;
      v = (v * 100 + (v >> 16)) & 0xffff0000ffff;
      v = (v * 10000 + (v >> 32)) & 0xffffffff;
      x = x * 100000000 + v;
      cur += 8;
    }

    for (; *cur >= '0'; ++cur) {
      CHECK(*cur <= '9');
      x = x * 10 + (*cur & 15);
    }
    CHECK(*cur <= 32);
    ++cur;
  }

  template <typename T>
  std::enable_if_t<my_type_traits::is_signed_v<T>, void> read(T& x) {
    bool neg = (*cur == '-');
    cur += neg;
    my_type_traits::make_unsigned_t<T> v;
    read(v);
    x = static_cast<T>(neg ? -v : v);
  }

  void read(char& c) {
    c = *cur++;
    ensure(1);
    CHECK(*cur <= 32);
    ++cur;
  }

  void read(std::string& s) {
    CHECK(*cur > 32);
#ifdef ENABLE_MMAP
    const char* first = cur;
    while (*cur > 32) ++cur;
    s.assign(first, cur);
    ++cur;
#else
    s.clear();
    while (true) {
      const char* last = cur;
      while (last < end && *last > 32) ++last;
      if (last < end) {
        s.append(cur, last);
        cur = last + 1;
        return;
      } else {
        s.append(cur, last);
        cur = end;
        ensure(1);
      }
    }
#endif
  }

  template <typename T>
  FastInput& operator>>(T& x) {
    skip_space();
    read(x);
    return *this;
  }

  FastInput& operator>>(char* s) {
    skip_space();
    while (*cur > 32) {
      *s++ = *cur++;
      ensure(1);
    }
    *s = 0;
    ++cur;
    return *this;
  }
};

struct EndLine {
} endl;

template <uint32_t BufSize = 1 << 20>
struct FastOutput {
  FILE* file;
  char* buf;
  char* cur;
  const char* end;
  char table[40000];

  explicit FastOutput(FILE* _file = stdout) : file(_file) {
    cur = buf = new char[BufSize];
    end = buf + BufSize;

    char* pos = table;
    for (char i = 48; i < 58; ++i) {
      for (char j = 48; j < 58; ++j) {
        for (char k = 48; k < 58; ++k) {
          for (char l = 48; l < 58; ++l) {
            *pos++ = i, *pos++ = j, *pos++ = k, *pos++ = l;
          }
        }
      }
    }
  }

  template <int N = BufSize>
  void flush() {
    if (__builtin_expect(end - cur < N, false)) {
      fwrite(buf, 1, cur - buf, file);
      cur = buf;
    }
  }

  ~FastOutput() {
    flush();
    delete[] buf;
  }

  template <bool head = false>
  std::enable_if_t<head, void> print_unit(uint32_t x) {
    int cnt = (x > 0) + (x > 9) + (x > 99) + (x > 999);
    memcpy(cur, table + (x << 2) + 4 - cnt, cnt);
    cur += cnt;
  }

  template <bool head = false>
  std::enable_if_t<!head, void> print_unit(uint32_t x) {
    memcpy(cur, table + (x << 2), 4);
    cur += 4;
  }

  template <int N, bool head = true, typename T>
  std::enable_if_t<(N > 0), void> print(T x) {
    print<N - 1, head>(x / 10000);
    print_unit<false>(x % 10000);
  }

  template <int N, bool head = true, typename T>
  std::enable_if_t<(N == 0), void> print(T x) {
    print_unit<head>(x);
  }

  template <typename T>
  std::enable_if_t<(sizeof(T) < 8), void> write(T x) {
    if (x > 9999'9999) {
      print<2>(x);
    } else if (x > 9999) {
      print<1>(x);
    } else {
      print<0>(x);
    }
  }

  template <typename T>
  std::enable_if_t<(sizeof(T) == 8), void> write(T x) {
    if (x > 9999'9999'9999'9999ull) {
      print<4>(x);
    } else if (x > 9999'9999'9999ull) {
      print<3>(x);
    } else if (x > 9999'9999) {
      print<2>(x);
    } else if (x > 9999) {
      print<1>(static_cast<uint32_t>(x));
    } else {
      print<0>(static_cast<uint32_t>(x));
    }
  }

  template <typename T>
  std::enable_if_t<(sizeof(T) > 8), void> write(T x) {
    static constexpr uint64_t limit1 = 1'0000'0000'0000'0000ull;
    static constexpr __uint128_t limit2 =
        static_cast<__uint128_t>(limit1) * static_cast<__uint128_t>(limit1);
    if (x < limit1) {
      write(static_cast<uint64_t>(x));
    } else if (x < limit2) {
      write(static_cast<uint64_t>(x / limit1));
      print<3, false>(static_cast<uint64_t>(x % limit1));
    } else {
      write(static_cast<uint32_t>(x / limit2));
      x %= limit2;
      print<3, false>(static_cast<uint64_t>(x / limit1));
      print<3, false>(static_cast<uint64_t>(x % limit1));
    }
  }

  template <typename T>
  std::enable_if_t<my_type_traits::is_unsigned_v<T>, FastOutput&> operator<<(
      T x) {
    flush<std::numeric_limits<T>::digits10 + 1>();
    if (x > 0) {
      write(x);
    } else {
      *cur++ = '0';
    }
    return *this;
  }

  template <typename T>
  std::enable_if_t<my_type_traits::is_signed_v<T>, FastOutput&> operator<<(
      T x) {
    using U = typename my_type_traits::make_unsigned_t<T>;

    flush<std::numeric_limits<T>::digits10 + 2>();
    if (x > 0) {
      write(static_cast<U>(x));
    } else if (x < 0) {
      *cur++ = '-';
      write(-static_cast<U>(x));
    } else {
      *cur++ = '0';
    }
    return *this;
  }

  FastOutput& operator<<(bool x) {
    flush<1>();
    *cur++ = x + '0';
    return *this;
  }

  FastOutput& operator<<(char c) {
    flush<1>();
    *cur++ = c;
    return *this;
  }

  FastOutput& operator<<(const char* s) {
    uint32_t len = strlen(s);
    if (__builtin_expect(len > BufSize, false)) {
      flush();
      do {
        fwrite(s, 1, BufSize, file);
        s += BufSize;
        len -= BufSize;
      } while (len > BufSize);
    } else if (__builtin_expect(end - cur < len, false)) {
      flush();
    }
    memcpy(cur, s, len);
    cur += len;
    return *this;
  }

  FastOutput& operator<<(char* s) {
    return *this << const_cast<const char*>(s);
  }

  FastOutput& operator<<(const std::string& s) {
    return *this << s.c_str();
  }

  FastOutput& operator<<(const EndLine& end_line) {
    flush<1>();
    *cur++ = '\n';
    flush();
    return *this;
  }
};

template <uint32_t InputBufSize = 1 << 20, uint32_t OutputBufSize = 1 << 20>
struct FastIO {
  FastInput<InputBufSize>* in;
  FastOutput<OutputBufSize>* out;

  FastIO() : in(nullptr), out(nullptr) {}

  ~FastIO() {
    if (in != nullptr) delete in;
    if (out != nullptr) {
      out->flush();
      delete out;
    }
  }

  void init(FILE* input_file = stdin, FILE* output_file = stdout) {
    in = new FastInput<InputBufSize>(input_file);
    out = new FastOutput<OutputBufSize>(output_file);
  }

  void flush() { out->flush(); }

  template <typename T>
  FastIO& operator>>(T& x) {
    *in >> x;
    return *this;
  }

  template <typename T>
  FastIO& operator<<(const T& x) {
    *out << x;
    return *this;
  }

  FastIO& operator<<(const EndLine& x) {
    *out << x;
    return *this;
  }
};

}  // namespace fast_io

using fast_io::FastIO;