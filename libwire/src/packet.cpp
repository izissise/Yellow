/*
 * Copyright 2016 <copyright holder> <email>
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 */

#include "packet.h"
#include "IpHeader.h"
#include "udp.h"
#include "tcp.h"

namespace Net {
	Packet::Packet(unsigned char* buffer) : ipHeader(Net::IpHeader(buffer)) {
		processPacket(buffer);
	}
	
	void Packet::processPacket(unsigned char *buffer) {
		unsigned int ip_header_len = ipHeader.ihl() * 4;
		ipHeader.debugDisplay();
		switch ((int)ipHeader.protocol())
		{
		case 1:  //ICMP Protocol
            //PrintIcmpPacket(Buffer,Size);
            break;
                  
        case 6: {
			//TCP Protocol
			Net::Tcp tcp = Net::Tcp(buffer + ip_header_len);
			tcp.debugDisplay();
			//print_tcp_packet(buffer , size);
            break;
		}         
        case 17: {
			//UDP Protocol
			Net::Udp udp = Net::Udp(buffer + ip_header_len);
			udp.debugDisplay();
            //print_udp_packet(buffer , size);
            break;
		}
        default:
            break;
		}		
	}
}
