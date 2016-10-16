//
//  OSXReaderPacket.cpp
//  Yellow
//
//  Created by Remi Robert on 16/10/2016.
//
//

#include "OSXReaderPacket.hpp"

#include <stdio.h>
#include <fcntl.h>

#include <string>
#include <vector>
#include <iostream>

#include <sys/types.h>
#include <sys/time.h>
#include <sys/ioctl.h>

#include <netinet/in.h>
#include <net/if.h>
#include <net/bpf.h>
#include <sys/socket.h>
#include <net/ethernet.h>
#include <arpa/inet.h>

#include <unistd.h>

struct ip_header {
    u_int8_t ihl;
    u_int8_t version;
    u_int8_t tos;
    u_int16_t tot_len;
    u_int16_t id;
    u_int16_t frag_off;
    u_int8_t ttl;
    u_int8_t protocol;
    u_int16_t check;
    u_int32_t saddr;
    u_int32_t daddr;
};

namespace Net {
    int open() {
        char buf[11] = {0};
        int bpf = 0;
        
        for(int i = 0; i < 99; i++) {
            sprintf( buf, "/dev/bpf%i", i );
            bpf = ::open( buf, O_RDWR );
            if( bpf != -1 )
                return bpf;
        }
        return -1;
    }
    
    int configure(int bpf) {
        size_t buf_len = 1;
        if (ioctl(bpf, BIOCSHDRCMPLT, &buf_len) < 0) {
            return -1;
        }
        if (ioctl(bpf, BIOCIMMEDIATE, &buf_len ) == -1) {
            return(-1);
        }
        if (ioctl( bpf, BIOCGBLEN, &buf_len ) == -1) {
            return(-1);
        }
        return (int)buf_len;
    }
    
    void OSXPacketReader::startListening(std::string const interfaceName) {
        struct ifreq bound_if;
        
        if ((_bpf = open()) == -1) {
            std::cout << "Error creation socket" << std::endl;
            return ;
        }
        
        std::strcpy(bound_if.ifr_name, interfaceName.c_str());
        if (ioctl(_bpf, BIOCSETIF, &bound_if ) > 0) {
            return ;
        }
        
        int buf_len;
        if ((buf_len = configure(_bpf)) == -1) {
            return ;
        }
    }
    
    void OSXPacketReader::stopListening() {
        close(_bpf);
    }
    
    void createIpHeader(unsigned char *buf) {
        printf("Mac address Ethernet Header\n");
        printf("%02x:%02x:%02x:%02x:%02x:%02x <- "
               "%02x:%02x:%02x:%02x:%02x:%02x\n",
               buf[0], buf[1], buf[2],
               buf[3], buf[4], buf[5],
               buf[6], buf[7], buf[8],
               buf[9], buf[10], buf[11]);
        
        struct sockaddr_in source;
        struct sockaddr_in dest;
        
        struct ip_header *iph = (struct ip_header *)&buf[12];
        
        memset(&source, 0, sizeof(source));
        source.sin_addr.s_addr = iph->saddr;
        
        memset(&dest, 0, sizeof(dest));
        dest.sin_addr.s_addr = iph->daddr;
        
        printf("IP Header\n");
        printf("   |-IP Version        : %d\n", (unsigned int)iph->version);
        printf("   |-IP Header Length  : %d DWORDS or %d Bytes\n", (unsigned int)iph->ihl,((unsigned int)(iph->ihl))*4);
        printf("   |-Type Of Service   : %d\n", (unsigned int)iph->tos);
        printf("   |-IP Total Length   : %d  Bytes(Size of Packet)\n", ntohs(iph->tot_len));
        printf("   |-Identification    : %d\n", ntohs(iph->id));
        printf("   |-TTL      : %d\n", (unsigned int)iph->ttl);
        printf("   |-Protocol : %d\n", (unsigned int)iph->protocol);
        printf("   |-Checksum : %d\n", ntohs(iph->check));
        printf("   |-Source IP        : %s\n", inet_ntoa(source.sin_addr));
        printf("   |-Destination IP   : %s\n", inet_ntoa(dest.sin_addr));
    }
    
    void OSXPacketReader::nextPacket() {
        unsigned char *buf = NULL;
        unsigned char *p = NULL;
        size_t blen = 0;
        ssize_t n = 0;
        
        if ((buf = (unsigned char *)malloc(blen)) == NULL)
            return;
        (void)printf("reading packets ...\n");
        (void)memset(buf, '\0', blen);
        n = read(_bpf, buf, blen);
        if (n <= 0)
            return;
        p = buf;
        
        std::cout << "😾 get new packet : " << n << std::endl;
        
        while (p < buf + n) {
            createIpHeader(buf);
            //            p += BPF_WORDALIGN(->bh_hdrlen + bh->bh_caplen);
        }
    }
}
