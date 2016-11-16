#include <catch.hpp>

#include "NetworkFrame.h"

static uint8_t d[] = "\x45\x00\x00\x3f\x28\xbc\x40\x00\x40\x06\xc8\x0e\xc0\xa8\x01\x4e\xc2\x84\xc5\x73\xfd\x12\x0f\xe6\x6d\xd7\xc5\xcc\x19\x65\xcc\x99\x80\x18\x01\x1c\x4a\x20\x00\x00\x01\x01\x08\x0a\x34\x42\xb1\xb3\x1c\xc7\xb3\x03\xcc\xeb\x32\xbd\xee\x5b\x32\xfb\xd7\xa2\x64";

static uint8_t dv6[] = "\x60\x0b\x77\xf1\x00\xb2\x06\x40\x20\x02\x84\x35\x0c\x2e\x48\x01\x54\xba\x06\x40\xb0\xb8\x8b\x72\x2a\x00\x10\x50\x40\x07\x08\x12\x00\x00\x00\x00\x00\x00\x21\x0e\x50\x08\x01\xbb\x8a\x12\x9c\x70\xd2\x0a\x6d\x85\x80\x18\x00\x2b\x20\xbc\x00\x00\x01\x01\x08\x0a\x34\x42\x7d\x1c\x5c\x62\x29\x56\x17\x03\x03\x00\x8d\x00\x00\x00\x00\x00\x00\x00\x06\xb5\x0a\xd7\x5b\xeb\xb2\xe1\x81\xe2\x6b\xb2\xb5\xe4\xa6\xa4\x0e\xbe\x25\xe6\x13\x09\x0e\xe5\x85\x63\x43\xa3\x1a\x25\x2c\x45\x9b\x02\x1a\xe5\x5d\xe8\x4e\x92\x24\xa1\xb0\x8c\x5c\x27\x0c\x50\xbf\xa6\x73\xa2\xc3\x60\x22\xef\x15\x10\x69\xbf\x5d\xc3\x73\xfc\x92\xae\x4b\xfa\x06\xc6\xc2\x10\x1f\xff\x7a\x92\xb1\xe9\x60\x67\x7d\x29\x9c\x0f\x2f\x74\x00\x68\x4c\xfa\xbd\x79\xfe\x54\x2e\xc2\xef\xea\x95\xdb\x4a\xb6\xd2\x54\x2b\xac\x17\x3a\x8b\x60\x18\x39\xed\x36\x75\x14\x50\x51\x27\x0d\xe6\x80\x8a\x52\xff\xc9\x93\x85\xac\xe6\x5c\x7c\xfb\x33";

TEST_CASE("Ip Headers tests", "[net][ip][packet]") {

    SECTION("Data should be the ones provided") {
        data_slice_t data(d, sizeof(d) -1);
        data_slice_t datav6(dv6, sizeof(dv6) -1);
        Net::IpHeaderV4 ipv4(data);
        Net::IpHeaderV6 ipv6(datav6);

        Net::INetworkFrame* ip = &ipv4;
        CHECK(ip->version() == Net::Version::V4);
        CHECK(ip->dstAddr().addr() == "194.132.197.115");
        CHECK(ip->srcAddr().addr() == "192.168.1.78");
        CHECK(ip->hopLimit() == 64);

        ip = &ipv6;
        CHECK(ip->version() == Net::Version::V6);
        CHECK(ip->dstAddr().addr() == "2a00:1050:4007:812::210e");
        CHECK(ip->srcAddr().addr() == "2002:8435:c2e:4801:54ba:640:b0b8:8b72");
        CHECK(ip->hopLimit() == 64);
    }

    SECTION("Should throw when header is not the right size") {
        uint8_t header[] = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x04\x00\x00\x00";
        uint8_t header2[] = "\x46\x00\x00\x3f\x28\xbc\x40\x00\x40\x06\xc8\x0e\xc0\xa8\x01\x4e\xc2\x84\xc5\x73";
        data_slice_t data(header, sizeof(header) - 1);
        data_slice_t data2(header2, sizeof(header2) - 1);
        REQUIRE_THROWS_AS(Net::IpHeaderV4 frame1(data), WrongSize);
        CHECK_THROWS_AS(Net::IpHeaderV4 frame2(data2), WrongSize);
        REQUIRE_THROWS_AS(Net::IpHeaderV6 frame3(data), WrongSize);
    }

    SECTION("Should set the right values") {
        data_slice_t data(d, sizeof(d) -1);
        data_slice_t datav6(dv6, sizeof(dv6) -1);
        Net::IpHeaderV4 ipv4(data);
        Net::IpHeaderV6 ipv6(datav6);

        ipv4.nextHeader(0x67);
        ipv6.nextHeader(0x00);
        REQUIRE_NOTHROW(ipv4.dstAddr(Net::NetAddr("192.168.1.1")));
        REQUIRE_NOTHROW(ipv6.dstAddr(Net::NetAddr("2502:8435:c2e:4801:54ba:640:b0b8:8b72")));
        REQUIRE_THROWS(ipv4.srcAddr(Net::NetAddr()));
        REQUIRE_THROWS(ipv4.srcAddr(Net::NetAddr("jflwef")));
        REQUIRE_THROWS(ipv4.srcAddr(Net::NetAddr("2002:8435:c2e:4801:54ba:640:b0b8:8b72")));
        REQUIRE_THROWS(ipv4.srcAddr(Net::NetAddr("345.983.1.3")));
        CHECK(ipv4.srcAddr().addr() == "192.168.1.78");
        CHECK(ipv4.dstAddr().addr() == "192.168.1.1");
        CHECK(ipv4.nextHeader() == 0x67);

        CHECK(ipv6.dstAddr().addr() == "2502:8435:c2e:4801:54ba:640:b0b8:8b72");
        CHECK(ipv6.nextHeader() == 0x00);
    }
}
