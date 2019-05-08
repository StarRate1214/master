#pragma once
#include "ids.h"

class CNonPayload
{
public:
    bool TTL(std::string ttl_opt,u_int8_t ttl);
    bool Tos(std::string tos_opt,u_int8_t tos);
    bool Fragbits(std::string fragbits_opt, bool mfrag, bool dfrag);
    bool dsize(std::string dsize_opt, u_int32_t dsize);
    bool flags(std::string flags_opt, bool fin, bool syn, bool rst, bool psh, bool ack, bool urg);
    bool seq(std::string seq_opt, u_int32_t seq_num);
    bool ack(std::string ack_opt, u_int32_t ack_num);
    bool window(std::string window_opt, u_int16_t win_size);
    bool itype(std::string itype_opt, u_int8_t icmp_type);
    bool icode(std::string icode_opt, u_int8_t icmp_code);
    //bool sameip(); RuleEngine에서 비교
};