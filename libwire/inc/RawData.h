#ifndef RAWDATA_H
#define RAWDATA_H

#include <cstdint>
#include <vector>
#include <iterator>
#include <string>

#include "DataSlice.h"

typedef std::vector<uint8_t> data_t;
typedef DataSlice<uint8_t>   data_slice_t;

template<typename Store>
data_t subData(Store const& d, size_t s) {
    auto start = std::begin(d);
    auto end = std::next(start, s);
    return data_t(start, end);
}

template<typename T, size_t N>
data_t subData(const T (&nb)[N], size_t s) {
    return data_t(nb, &(nb[s]));
}

#endif // RAWDATA_H
