#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <cstdlib>
#include <thread>
#include <queue>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <net/ethernet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/if_ether.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>

#include <fstream>
#include <time.h>
#include <cstring>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include<mutex>
#include<unordered_map>

#include<pcap/pcap.h>

//content http option
#define HTTP_CLIENT_BODY    0x40
#define HTTP_COOKIE         0x20
#define HTTP_HEADER         0x10
#define HTTP_METHOD         0x08
#define HTTP_URI            0x04
#define HTTP_STAT_CODE      0x02
#define HTTP_STAT_MSG       0x01

enum
{
    //Packet.h
    TCP,
    UDP,
    ICMP,
    //Rule.h IP,port option
    NOT,
    COMM,
    ANY,
    //Rule.h Option
    CONTENTS,
    PCRE,
    NPTTL,
    NPTOS,
    NPFBITS,
    NPDSIZE,
    NPFLAGS,
    NPSEQ,
    NPACK,
    NPWINDOW,
    NPITYPE,
    NPICODE,
    NPSAMEIP,
    NATION,
    //detection_filter.h
    SRC,
    DST
};