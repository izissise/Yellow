#include <catch.hpp>

#include "Tcp.h"

static uint8_t d[] = "\xfd\x12\x0f\xe6\x6d\xd7\xc5\xcc\x19\x65\xcc\x99\x80\x18\x01\x1c\x4a\x20\x00\x00\x01\x01\x08\x0a\x34\x42\xb1\xb3\x1c\xc7\xb3\x03\xcc\xeb\x32\xbd\xee\x5b\x32\xfb\xd7\xa2\x64";

TEST_CASE("Tcp Headers tests", "[net][tcp][protocol][packet]") {

    SECTION("Data should be the ones provided") {
        data_slice_t data(d, sizeof(d) -1);
        Net::Tcp tcp(data);

        Net::ATransport* tr = &tcp;
        CHECK(tr->type() == Net::Transport::TCP);
        CHECK(tcp.dstPort() == 4070);
        CHECK(tcp.srcPort() == 64786);
        CHECK(tcp.flags() == 24);
        CHECK(tcp.ack() == 426101913);
        CHECK(tcp.offset() == 8);
        CHECK(tcp.seq() == 1842857420);
        CHECK(tcp.sum() == 18976);
        CHECK(tcp.urg() == 0);
        CHECK(tcp.window() == 284);
    }

    SECTION("Should throw when header is not the right size") {
        uint8_t header[] = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x04\x00\x00\x00";
        data_slice_t data(header, sizeof(header) - 1);
        REQUIRE_THROWS_AS(Net::Tcp t(data), WrongSize);
    }

    SECTION("Should set the right values") {
        data_slice_t data(d, sizeof(d) -1);
        Net::Tcp tcp(data);

        tcp.srcPort(7465);
        CHECK(tcp.srcPort() == 7465);
        CHECK(tcp.dstPort() == 4070);
    }
}
