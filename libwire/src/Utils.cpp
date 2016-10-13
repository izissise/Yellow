#include "Utils.h"

#include <errno.h>

std::system_error lastSystemError() {
    return std::system_error(errno, std::system_category());
}
