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
    NPICODE
};

