#ifndef DATASLICE_H
#define DATASLICE_H

#include <stdexcept>
#include <type_traits>
#include <cstdint>

template<typename DataType, typename std::enable_if<std::is_pod<DataType>::value, int>::type = 0>
class DataSlice {
public:
    DataSlice(DataType* ptr, size_t size)
    : _ptr(ptr), _size(size) {
    }

    template<typename Container>
    DataSlice(Container const& cont)
    : _ptr(cont.data()), _size(cont.size()) {
    }

    template<typename Container>
    DataSlice(Container& cont)
    : _ptr(cont.data()), _size(cont.size()) {
    }

    ~DataSlice() = default;

    template<typename U = DataType>
    void operator+=(size_t size) {
        if (size > _size) {
            throw std::runtime_error("Trying to move out of slice");
        }
        _ptr = &(_ptr[size]);
        _size -= size;
    }

    template<typename U = DataType>
    DataSlice<U>& operator+(size_t size) const {
        DataSlice<U> ret(*this);
        ret += size;
        return ret;
    }

    DataType* ptr() const { return _ptr; }
    size_t size() const { return _size; }

private:
    DataType* _ptr;
    size_t    _size;
};

#endif // DATASLICE_H
