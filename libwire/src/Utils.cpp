#include "Utils.h"

#include <sstream>
#include <iomanip>

#include <errno.h>

std::system_error lastSystemError() {
    return std::system_error(errno, std::system_category());
}

std::string stringToHex(std::string const& s, std::string separator, bool uppercase) {
    std::ostringstream ret;
    for (std::string::size_type i = 0; i < s.length(); ++i) {
        ret << std::hex
        << std::setfill('0')
        << std::setw(2)
        << (uppercase ? std::uppercase : std::nouppercase)
        << static_cast<int>(static_cast<uint8_t>(s[i]));
        if (separator.size() > 0 && i < s.length() - 1) {
            ret << separator;
        }
    }
    return ret.str();
}
