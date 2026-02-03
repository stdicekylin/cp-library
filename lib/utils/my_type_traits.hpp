#pragma once

namespace internal {

template <typename T>
struct make_unsigned : std::make_unsigned<std::remove_cvref_t<T>> {};

#ifdef __SIZEOF_INT128__

template <typename T>
  requires(std::same_as<std::remove_cvref_t<T>, __int128_t> ||
           std::same_as<std::remove_cvref_t<T>, __uint128_t>)
struct make_unsigned<T> {
  using type = __uint128_t;
};

template <typename T>
concept signed_integral = std::signed_integral<std::remove_cvref_t<T>> ||
                          std::same_as<std::remove_cvref_t<T>, __int128_t>;

template <typename T>
concept unsigned_integral = std::unsigned_integral<std::remove_cvref_t<T>> ||
                            std::same_as<std::remove_cvref_t<T>, __uint128_t>;

template <typename T>
concept integral = signed_integral<T> || unsigned_integral<T>;

#else

template <typename T>
concept signed_integral = std::signed_integral<std::remove_cvref_t<T>>;

template <typename T>
concept unsigned_integral = std::unsigned_integral<std::remove_cvref_t<T>>;

template <typename T>
concept integral = std::integral<std::remove_cvref_t<T>>;

#endif

template <typename T>
using make_unsigned_t = typename make_unsigned<T>::type;

}  // namespace internal