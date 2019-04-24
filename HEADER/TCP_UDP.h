#include <sys/types.h>

class CTCP
{
private:
    u_int16_t   src_port;
    u_int16_t   dst_port;
    u_int32_t   seq_num;
    u_int32_t   ack_num;
    u_int8_t    ihl:4;
    bool        urg;
    bool        ack;
    bool        psh;
    bool        rst;
    bool        syn;
    bool        fin;
    u_int16_t   win_size;
public:
    CTCP(   u_int16_t   src_port,
            u_int16_t   dst_port,
            u_int32_t   seq_num,
            u_int32_t   ack_num,
            u_int8_t    ihl,
            bool        urg,
            bool        ack,
            bool        psh,
            bool        rst,
            bool        syn,
            bool        fin,
            u_int16_t   win_size );
    CTCP(const CTCP &ref);
    CTCP &operator=(const CTCP &tcp);
    CTCP();
    ~CTCP();
    inline u_int16_t    getSrcPort(void) { return src_port; };
    inline u_int16_t    getDstPort(void) { return dst_port; };
    inline u_int32_t    getSeqNum(void) { return seq_num; };
    inline u_int32_t    getAckNum(void) { return ack_num; };
    inline u_int8_t     getIhl(void) { return ihl; };
    inline bool         getUrg(void) { return urg; };
    inline bool         getAck(void) { return ack; };
    inline bool         getPsh(void) { return psh; };
    inline bool         getRst(void) { return rst; };
    inline bool         getSyn(void) { return syn; };
    inline bool         getFin(void) { return fin; };
    inline u_int16_t    getWinSize(void) { return win_size; };

};

class CUDP
{
private:
    u_int16_t   src_port;
    u_int16_t   dst_port;
public:
    CUDP(u_int16_t src_port, u_int16_t dst_port);
    CUDP(const CUDP &ref);
    CUDP &operator=(const CUDP &udp);
    CUDP();
    ~CUDP();
    inline u_int16_t getSrcPort(void) { return src_port; }
    inline u_int16_t getDstPort(void) { return dst_port; }
};