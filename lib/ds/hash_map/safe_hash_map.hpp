#pragma once

#include "lib/ds/hash_map/hash_map_core.hpp"
#include "lib/math/hash/splitmix64.hpp"

template <typename K, typename V, uint32_t N>
using SafeHashMap = HashMapCore<K, V, N, SplitMix64>;