#pragma once

namespace my_type_traits {

template <class T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

template <class T>
struct make_unsigned : std::make_unsigned<remove_cvref_t<T>> {};

template <class T>
using make_unsigned_t = typename make_unsigned<T>::type;

template <class T>
struct is_signed : std::is_signed<remove_cvref_t<T>> {};

template <class T>
constexpr bool is_signed_v = is_signed<T>::value;

template <class T>
struct is_unsigned : std::is_unsigned<remove_cvref_t<T>> {};

template <class T>
constexpr bool is_unsigned_v = is_unsigned<T>::value;

template <class T>
struct is_integral : std::is_integral<remove_cvref_t<T>> {};

template <class T>
constexpr bool is_integral_v = is_integral<T>::value;

#ifdef __SIZEOF_INT128__

template <>
struct make_unsigned<__int128_t> { using type = __uint128_t; };

template <>
struct make_unsigned<__uint128_t> { using type = __uint128_t; };

template <>
struct is_signed<__int128_t> : std::true_type {};

template <>
struct is_unsigned<__uint128_t> : std::true_type {};

template <>
struct is_integral<__int128_t> : std::true_type {};

template <>
struct is_integral<__uint128_t> : std::true_type {};

#endif

}  // namespace my_type_traits