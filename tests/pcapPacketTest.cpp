#include <catch.hpp>

#include "PcapPacket.h"
#include <unistd.h>

TEST_CASE("Pcap pcackets tests", "[net][pcap][date]") {

    SECTION("Data should be the ones provided") {
        char d[] = "\x85\x65\x57\xb5";
        data_t data(d, sizeof(d) - 1);
        Net::PcapPacket packet(data);
        CHECK(packet.packet() == data);
        CHECK(packet.size() == (sizeof(d) - 1));
        CHECK(packet.isIncomplete() == false);
    }

    SECTION("Date should be the one provided") {
        char d[] = "\x85\x65\x57\xb5";
        data_t data(d, sizeof(d) - 1);
        std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();
        Net::PcapPacket packet(data, now);
        CHECK(packet.date() == std::chrono::time_point_cast<std::chrono::microseconds>(now));
    }

    SECTION("Raw data should match") {
        char d[] = "\x85\x35\x57\xb5";
        char header[] = "\x00\x00\x00\x00\x00\x00\x00\x00\x04\x00\x00\x00\x04\x00\x00\x00";
        data_t data(d, sizeof(d) - 1);
        auto date = std::chrono::system_clock::from_time_t(0);
        Net::PcapPacket packet(data, date);

        data_t raw = data_t(header, sizeof(header) - 1) + data;
        CHECK(packet.date() == date);
        CHECK(packet.getRawData() == raw);
    }

}