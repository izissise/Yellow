#include <catch.hpp>

#include <sstream>

#include "PcapPacket.h"

#include <unistd.h>

TEST_CASE("Pcap pcackets tests", "[net][pcap][date]") {

    SECTION("Data should be the ones provided") {
        uint8_t d[] = "\x85\x65\x57\xb5";
        data_t data(d, sizeof(d) - 1);
        Net::PcapPacket packet(data);
        CHECK(packet.packet() == data);
        CHECK(packet.size() == (sizeof(d) - 1));
        CHECK(packet.isIncomplete() == false);
    }

    SECTION("Date should be the one provided") {
        uint8_t d[] = "\x85\x65\x57\xb5";
        data_t data(d, sizeof(d) - 1);
        std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();
        Net::PcapPacket packet(data, now);
        CHECK(packet.date() == std::chrono::time_point_cast<std::chrono::microseconds>(now));
    }

    SECTION("Raw data should match") {
        uint8_t d[] = "\x85\x35\x57\xb5";
        uint8_t header[] = "\x00\x00\x00\x00\x00\x00\x00\x00\x04\x00\x00\x00\x04\x00\x00\x00";
        data_t data(d, sizeof(d) - 1);
        auto date = std::chrono::system_clock::from_time_t(0);
        Net::PcapPacket packet(data, date);

        data_t raw = data_t(header, sizeof(header) - 1) + data;
        CHECK(packet.date() == date);
        CHECK(packet.getRawData() == raw);
    }

    SECTION("Should throw when header is not the right size") {
        std::stringstream ss;

        uint8_t header[] = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x04\x00\x00\x00";
        ss.write(reinterpret_cast<const char*>(header), sizeof(header) - 1);
        REQUIRE_THROWS_AS(Net::PcapPacket(ss, false, 2000), WrongSize);
    }

    SECTION("Should throw when the size in header is not the right size") {
        std::stringstream ss;
        uint8_t d[] = "\x85\x35\x57";
        uint8_t header[] = "\x00\x00\x00\x00\x00\x00\x00\x00\x04\x00\x00\x00\x04\x00\x00\x00";
        data_t data(d, sizeof(d) - 1);
        data_t raw = data_t(header, sizeof(header) - 1) + data;
        ss.write(reinterpret_cast<const char*>(raw.data()), raw.size());
        REQUIRE_THROWS_AS(Net::PcapPacket(ss, false, 2000), WrongSize);
    }
}
