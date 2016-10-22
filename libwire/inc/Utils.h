#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <memory>
#include <exception>
#include <system_error>

#include "RawData.h"

std::system_error lastSystemError();
std::string stringToHex(std::string const& s, bool uppercase = true);

//! Switch number endianess
//! @param nb The number
//! @return the switched number
template<typename T>
T switchEndianness(T nb) {
    union
    {
        T u;
        unsigned char u8[sizeof(T)];
    } source, dest;
    source.u = nb;

    for (size_t k = 0; k < sizeof(T); k++)
        dest.u8[k] = source.u8[sizeof(T) - k - 1];
    return dest.u;
}

//! @class WrongSize
//! An error class throwed when not enought bytes are present
class WrongSize : public std::exception {
public:
    explicit WrongSize(const char* message, size_t missingByte, size_t typeSize)
    : _message(message), _missingByte(missingByte), _typeSize(typeSize) {
        _message += " Wanted Size: " + std::to_string(_typeSize)
        + " Missing Bytes: " + std::to_string(_missingByte);
    }

    const char* what() const noexcept override {
        return _message.c_str();
    }

    size_t getMissingSize() const { return _missingByte; };
    size_t getTypeSize() const { return _typeSize; };

    virtual ~WrongSize() throw() {}
private:
    std::string _message;
    size_t _missingByte;
    size_t _typeSize;
};

//! Get from a databuffer the corresponding number doing endianess convertion if necessary
//! @tparam T The type of nb
//! @tparam SIZE The size of the number in byte
//! @param bytes The databuffer
//! @param nb The number to get from the databuffer
//! @tparam BUFF The buffer type
template<typename T, size_t SIZE = sizeof(T), typename BUFF>
void readBytes(const BUFF &bytes, size_t &pos, T &nb, bool swap) {
    static_assert(sizeof(T) >= SIZE, "The specified size must be inferior or equal to the type real size.");
    if (bytes.size() < (pos + SIZE)) {
        throw WrongSize("readBytes: Buffer not large enough for requested type.", (pos + SIZE) - bytes.size(), SIZE);
    }

    nb = 0;
    if (swap) {
        for (size_t i = 0; i < SIZE; ++i) {
            nb = static_cast<T>((nb << 8) | static_cast<uint8_t>(bytes[pos + i]));
        }
    } else {
        for (size_t i = 0; i < SIZE; ++i) {
            nb = static_cast<T>(nb | static_cast<uint8_t>(bytes[pos + i]));
        }
    }
    pos += SIZE;
}

//! Create a ressource and bind the function which destroy it
//! @param c The function which create the ressource
//! @param d The function which destroy the ressource
//! @param args The arguments to use with the construction function
//! @return An unique_ptr with the destructor set to d
template<typename Creator, typename Destructor, typename... Arguments>
auto make_resource(Creator c, Destructor d, Arguments&&... args)
-> std::unique_ptr<typename std::decay<decltype(*c(std::forward<Arguments>(args)...))>::type, Destructor> {
    auto r = c(std::forward<Arguments>(args)...);
    if (!r) { throw std::runtime_error{"Unable to create resource"}; }
    typedef typename std::decay<decltype(*r)>::type ResourceType;
    return std::unique_ptr<ResourceType, Destructor>(r, d);
}

#endif // UTILS_H
