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

enum Variable
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
    DST,
    //Rule Action
    ALERT,
    LOG,
    PASS,
    //Protocols
    INSERT,
    UPDATE,
    DELETE
};

enum ErrorCode
{
    S_SOCKET_ERROR, //소켓 생성 에러
    S_BIND_ERROR,
    S_LISTEN_ERROR,
    S_ACCEPT_ERROR,
    C_FILE_ERROR,//설정 파일 읽기 에러
    C_DBINFO_ERROR, //설정 파일에 DB정보 설정 오류
    C_GEOINFO_ERROR,//설정 파일에 geoinfo용 DB 정보 설정 오류
    C_INTERFACE_ERROR,//설정 파일에 인터페이스 정보 설정 오류
    C_VARIABLES_ERROR//설정 파일에 변수 정보 설정 오류
};