#include <catch.hpp>

#include "Hello.h"

TEST_CASE("Hello Test", "[test]") {

    SECTION("Hello") {
        Hello hi;
        CHECK(hi.yo() == "Hello");
    }
}
