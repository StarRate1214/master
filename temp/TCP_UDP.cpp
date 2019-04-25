#include "TCP_UDP.h"
CTCP ::CTCP()
{
}
CTCP ::~CTCP()
{
}
CTCP ::CTCP(u_int16_t src_port,
            u_int16_t dst_port,
            u_int32_t seq_num,
            u_int32_t ack_num,
            u_int8_t ihl,
            bool urg,
            bool ack,
            bool psh,
            bool rst,
            bool syn,
            bool fin,
            u_int16_t win_size)
{
    this->src_port = src_port;
    this->dst_port = dst_port;
    this->seq_num = seq_num;
    this->ack_num = ack_num;
    this->ihl = ihl;
    this->urg = urg;
    this->ack = ack;
    this->psh = psh;
    this->rst = rst;
    this->syn = syn;
    this->fin = fin;
    this->win_size = win_size;
}
CTCP ::CTCP(const CTCP &ref)
{
    src_port = ref.src_port;
    dst_port = ref.dst_port;
    seq_num = ref.seq_num;
    ack_num = ref.ack_num;
    ihl = ref.ihl;
    urg = ref.urg;
    ack = ref.ack;
    psh = ref.psh;
    rst = ref.rst;
    syn = ref.syn;
    fin = ref.fin;
    win_size = ref.win_size;
}
CTCP &CTCP ::operator=(const CTCP &ref)
{
    src_port = ref.src_port;
    dst_port = ref.dst_port;
    seq_num = ref.seq_num;
    ack_num = ref.ack_num;
    ihl = ref.ihl;
    urg = ref.urg;
    ack = ref.ack;
    psh = ref.psh;
    rst = ref.rst;
    syn = ref.syn;
    fin = ref.fin;
    win_size = ref.win_size;

    return *this;
}
void CTCP ::setSrcPort(u_int16_t src_port) { this->src_port = src_port; }
void CTCP ::setDstPort(u_int16_t dst_port) { this->dst_port = dst_port; }
void CTCP ::setSeqNum(u_int32_t seq_num) { this->seq_num = seq_num; }
void CTCP ::setAckNum(u_int32_t ack_num) { this->ack_num = ack_num; }
void CTCP ::setIhl(u_int8_t ihl) { this->ihl = ihl; }
void CTCP ::setUrg(bool urg) { this->urg = urg; }
void CTCP ::setAck(bool ack) { this->ack = ack; }
void CTCP ::setPsh(bool psh) { this->psh = psh; }
void CTCP ::setRst(bool rst) { this->rst = rst; }
void CTCP ::setSyn(bool syn) { this->syn = syn; }
void CTCP ::setFin(bool fin) { this->fin = fin; }
void CTCP ::setWinSize(u_int16_t win_size) { this->win_size = win_size; }

CUDP ::CUDP()
{
}
CUDP ::~CUDP()
{
}
CUDP ::CUDP(u_int16_t src_port, u_int16_t dst_port)
{
    this->src_port = src_port;
    this->dst_port = dst_port;
}
CUDP ::CUDP(const CUDP &ref)
{
    src_port = ref.src_port;
    dst_port = ref.dst_port;
}
CUDP &CUDP ::operator=(const CUDP &ref)
{
    src_port = ref.src_port;
    dst_port = ref.dst_port;
}
void CUDP ::setSrcPort(u_int16_t src_port) { this->src_port = src_port; }
void CUDP ::setDstPort(u_int16_t dst_port) { this->dst_port = dst_port; }