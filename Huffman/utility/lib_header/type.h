#pragma once
#include <utility>
#include <cstdint>
#include <cstdlib>

typedef unsigned char machine_word;
typedef std::pair<int64_t, size_t> node;

const int BUFF_SIZE = 10000;
const int MAX_SIZE = 10000;
const int64_t SIZE_BLOCK = 1 << 8;
const int64_t INF = static_cast<int64_t>(1) << 62;