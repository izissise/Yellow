#include <catch.hpp>

#include "Hello.hpp"

TEST_CASE("Hello Test", "[test]") {

    SECTION("Hello") {
        Hello hi;
        CHECK(hi.yo() == "Hello");
    }
}
