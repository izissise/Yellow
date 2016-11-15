#include <catch.hpp>

#include "EthernetFrame.h"

static uint8_t d[] = "\x44\xce\x7d\xb0\xb1\x50\x10\x08\xb1\x60\xb2\xff\x08\x00\x45\x00\x00\x3f\x28\xbc\x40\x00\x40\x06\xc8\x0e\xc0\xa8\x01\x4e\xc2\x84\xc5\x73\xfd\x12\x0f\xe6\x6d\xd7\xc5\xcc\x19\x65\xcc\x99\x80\x18\x01\x1c\x4a\x20\x00\x00\x01\x01\x08\x0a\x34\x42\xb1\xb3\x1c\xc7\xb3\x03\xcc\xeb\x32\xbd\xee\x5b\x32\xfb\xd7\xa2\x64";

TEST_CASE("Ethernet tests", "[net][ethernet][packet]") {

    SECTION("Data should be the ones provided") {
        data_slice_t data(d, sizeof(d) -1);
        Net::EthernetFrame frame(data);

        CHECK(frame.dstAddr() == "44:CE:7D:B0:B1:50");
        CHECK(frame.srcAddr() == "10:08:B1:60:B2:FF");
        CHECK(frame.type() == 0x0800);
    }

    SECTION("Should throw when header is not the right size") {
        uint8_t header[] = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x04\x00\x00\x00";
        data_slice_t data(header, sizeof(header) - 1);
        REQUIRE_THROWS_AS(Net::EthernetFrame frame(data), WrongSize);
    }

    SECTION("Should set the right values") {
        uint8_t shouldBe[] = "\x00\xbe\x5f\x97\x3d\x2c\x10\x08\xb1\x60\xb2\xff\x78\x52\x45\x00\x00\x3f\x28\xbc\x40\x00\x40\x06\xc8\x0e\xc0\xa8\x01\x4e\xc2\x84\xc5\x73\xfd\x12\x0f\xe6\x6d\xd7\xc5\xcc\x19\x65\xcc\x99\x80\x18\x01\x1c\x4a\x20\x00\x00\x01\x01\x08\x0a\x34\x42\xb1\xb3\x1c\xc7\xb3\x03\xcc\xeb\x32\xbd\xee\x5b\x32\xfb\xd7\xa2\x64";

        //Copy data
        uint8_t dcpy[sizeof(d)];
        std::memcpy(dcpy, d, sizeof(d));
        data_slice_t data(dcpy, sizeof(d) -1);
        Net::EthernetFrame frame(data);

        frame.type(0x7852);
        CHECK(frame.type() == 0x7852);
        REQUIRE_NOTHROW(frame.dstAddr("00:be:5F:97:3d:2c"));
        CHECK_THROWS_AS(frame.srcAddr("67:0:be:3d:2c"), std::runtime_error);
        CHECK_THROWS_AS(frame.srcAddr("934"), std::runtime_error);
        CHECK_THROWS_AS(frame.srcAddr("gdgrt"), std::runtime_error);
        CHECK_THROWS_AS(frame.srcAddr(""), std::runtime_error);
        CHECK(frame.dstAddr() == "00:BE:5F:97:3D:2C");
        CHECK(frame.srcAddr() == "10:08:B1:60:B2:FF");
        CHECK(std::string(dcpy, &(dcpy[sizeof(dcpy)])) == std::string(shouldBe, &(shouldBe[sizeof(shouldBe)])));
    }
}
