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
#include <syslog.h>

#include <mutex>
#include <unordered_map>

#include <pcap/pcap.h>

//content http option
#define HTTP_CLIENT_BODY 0x40
#define HTTP_COOKIE 0x20
#define HTTP_HEADER 0x10
#define HTTP_METHOD 0x08
#define HTTP_URI 0x04
#define HTTP_STAT_CODE 0x02
#define HTTP_STAT_MSG 0x01

enum PROTOCOL
{
    //Packet.h
    TCP,
    UDP,
    ICMP
};

enum HeaderOption{
    NOT,
    COMM,
    ANY
};

enum RuleOption{
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
    NATION
};

enum DetectionFilter{
    SRC,
    DST,
    ALL
};

enum RuleAction{
    ALERT,
    LOG,
    PASS
};

enum ModRule{
    INSERT,
    UPDATE,
    DELETE
};

enum ErrorCode
{
    S_SOCKET_ERROR,    //소켓 생성 에러
    S_BIND_ERROR,      //소켓 바인드 에러
    S_LISTEN_ERROR,    //소켓 리스팅 에러
    S_ACCEPT_ERROR,    //소켓 엑셉트 에러
    C_FILE_ERROR,      //설정 파일 읽기 에러
    C_DBINFO_ERROR,    //설정 파일에 DB정보 설정 오류
    C_GEOINFO_ERROR,   //설정 파일에 geoinfo용 DB 정보 설정 오류
    C_INTERFACE_ERROR, //설정 파일에 인터페이스 정보 설정 오류
    D_GETRULE_ERROR, //db에서 룰 가져오는 것 실패
    D_GETVARIABLE_ERROR//db에서 변수 정보 가져오는 것 실패
};

enum ObserverMap{
    SET_SOURCE,
    SET_DEST
};

typedef struct{
    int ipOpt;                   //Source IP option
    u_int32_t ip;                //Source IP
    u_int32_t netmask;           //Source IP netmask
}IP_value;

typedef struct{
    int portOpt;                 //Source port option
    std::vector<u_int16_t> port;
}Port_value;