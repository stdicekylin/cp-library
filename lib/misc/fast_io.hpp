#pragma once

#include "lib/debug.hpp"
#include "lib/misc/my_type_traits.hpp"

#ifdef __unix__
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

namespace fast_io {

struct FastInput {
  static constexpr size_t Offset = 16;

  char* buf;
  const char* cur;
  const char* end;

#ifdef __unix__
  size_t map_size;
#endif

  explicit FastInput(FILE* file = stdin)
      : buf(nullptr),
        cur(nullptr),
        end(nullptr)
#ifdef __unix__
        ,
        map_size(0)
#endif
  {
#ifdef __unix__
    struct stat status;
    int fd = fileno(file);
    fstat(fd, &status);
    map_size = static_cast<size_t>(status.st_size);
    cur = static_cast<const char*>(
        mmap(nullptr, map_size, PROT_READ, MAP_PRIVATE, fd, 0));
    end = cur + map_size;
#else
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    cur = buf = new char[file_size + Offset];
    end = buf + fread(buf, 1, file_size, file);
    memset(const_cast<char*>(end), 0, Offset);
#endif
  }

  ~FastInput() {
#ifdef __unix__
    if (cur != nullptr) {
      munmap(const_cast<char*>(cur), map_size);
    }
#else
    delete[] buf;
#endif
  }

  constexpr bool is_digit() const {
    return (*cur > 47) & (*cur < 58);
  }

  void skip_space() {
    CHECK(cur < end);
    while (*cur < 33) {
      ++cur;
      CHECK(cur < end);
    }
  }

  void read(bool& x) { x = static_cast<bool>(*cur++ & 15); }

  template <typename T>
  std::enable_if_t<my_type_traits::is_unsigned_v<T>, void> read(T& x) {
    CHECK(is_digit());
    x = *cur++ & 15;

    static constexpr bool check8 = (std::numeric_limits<T>::digits10 >= 8);
    static constexpr bool check4 = (std::numeric_limits<T>::digits10 >= 4);
    static constexpr bool check2 = (std::numeric_limits<T>::digits10 >= 2);

    while (check8) {
      uint64_t v;
      memcpy(&v, cur, 8);
      if ((v -= 0x3030303030303030) & 0x8080808080808080) break;
      v = (v * 10 + (v >> 8)) & 0xff00ff00ff00ff;
      v = (v * 100 + (v >> 16)) & 0xffff0000ffff;
      v = (v * 10000 + (v >> 32)) & 0xffffffff;
      x = x * 100000000 + v;
      cur += 8;
    }

    if (check4) {
      uint32_t v;
      memcpy(&v, cur, 4);
      if (!((v -= 0x30303030) & 0x80808080)) {
        v = (v * 10 + (v >> 8)) & 0xff00ff;
        v = (v * 100 + (v >> 16)) & 0xffff;
        x = 10000 * x + v;
        cur += 4;
      }
    }

    if (check2) {
      uint16_t v;
      memcpy(&v, cur, 2);
      if (!((v -= 0x3030) & 0x8080)) {
        v = (v * 10 + (v >> 8)) & 0xff;
        x = x * 100 + v;
        cur += 2;
      }
    }

    if (is_digit()) {
      x = x * 10 + (*cur++ & 15);
    }
  }

  template <typename T>
  std::enable_if_t<my_type_traits::is_signed_v<T>, void> read(T& x) {
    bool neg = (*cur == '-');
    cur += neg;
    my_type_traits::make_unsigned_t<T> v;
    read(v);
    x = static_cast<T>(neg ? -v : v);
  }

  void read(char& c) { c = *cur++; }

  void read(std::string& s) {
    const char* first = cur;
    while (*cur > 32) ++cur;
    s.assign(first, cur);
  }

  template <typename T>
  FastInput& operator>>(T& x) {
    skip_space();
    read(x);
    return *this;
  }

  FastInput& operator>>(char* s) {
    skip_space();
    while (*cur > 32) *s++ = *cur++;
    *s = 0;
    return *this;
  }
};

struct EndLine {
} endl;

template <uint32_t BufSize = 1 << 20, uint32_t MaxDigits = 40>
struct FastOutput {
  FILE* file;
  char buf[BufSize];
  char* cur;
  const char* end;
  char aux[MaxDigits];
  char table[40000];

  explicit FastOutput(FILE* _file = stdout)
      : file(_file), cur(buf), end(buf + BufSize) {
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

  void flush() {
    fwrite(buf, 1, cur - buf, file);
    cur = buf;
  }

  ~FastOutput() { flush(); }

  FastOutput& operator<<(bool x) {
    if (__builtin_expect(cur == end, false)) {
      flush();
    }
    *cur++ = x + '0';
    return *this;
  }

  template <typename T>
  std::enable_if_t<my_type_traits::is_integral_v<T>, FastOutput&> operator<<(
      T x) {
    using U = my_type_traits::make_unsigned_t<T>;

    if (__builtin_expect(end - cur <= MaxDigits, false)) {
      flush();
    }

    U num = static_cast<U>(x);

    if (num == 0) {
      *cur++ = '0';
      return *this;
    }

    if (x < 0) {
      num = -num;
      *cur++ = '-';
    }

    char* pos = aux + MaxDigits;
    int count4 = std::numeric_limits<T>::digits10 / 4;
    for (int i = 0; i < count4; ++i) {
      if (num > 999) {
        memcpy(pos -= 4, table + num % 10000 * 4, 4);
        num /= 10000;
      }
    }
    int cnt = (num > 0) + (num > 9) + (num > 99) + (num > 999);
    memcpy(pos -= cnt, table + num * 4 + 4 - cnt, cnt);
    int len = aux + MaxDigits - pos;
    memcpy(cur, pos, len);
    cur += len;
    return *this;
  }

  FastOutput& operator<<(char c) {
    if (__builtin_expect(cur == end, false)) {
      flush();
    }
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
    if (__builtin_expect(cur == end, false)) {
      flush();
    }
    *cur++ = '\n';
    flush();
    return *this;
  }
};

template <uint32_t BufSize = 1 << 20, uint32_t MaxDigits = 40>
struct FastIO {
  FastInput* in;
  FastOutput<BufSize, MaxDigits>* out;

  FastIO() : in(nullptr), out(nullptr) {}

  ~FastIO() {
    if (in != nullptr) delete in;
    if (out != nullptr) {
      out->flush();
      delete out;
    }
  }

  void init(FILE* input_file = stdin, FILE* output_file = stdout) {
    in = new FastInput(input_file);
    out = new FastOutput<BufSize, MaxDigits>(output_file);
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

#define endl fast_io::endl

using fast_io::FastIO;