#include <catch.hpp>

#include "NetUtils.h"

TEST_CASE("Interface Listing", "[net][interface]") {

    SECTION("Localhost exist") {
        bool exist = false;
        bool hasGAddr = false;

        for (auto const& ifa : Net::listInterfaces()) {
            if (ifa.isLoopback()) {
                exist = true;
                if (ifa.getAddr() == Net::NetAddr("127.0.0.1")) {
                    hasGAddr = true;
                    break;
                }
            }
        }
        CHECK(exist);
        CHECK(hasGAddr);
    }
}
