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
    CTCP();
    ~CTCP();
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
    CTCP &operator=(const CTCP &ref);
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
    void setSrcPort(u_int16_t src_port);
    void setDstPort(u_int16_t dst_port);
    void setSeqNum(u_int32_t seq_num);
    void setAckNum(u_int32_t ack_num);
    void setIhl(u_int8_t ihl);
    void setUrg(bool urg);
    void setAck(bool ack);
    void setPsh(bool psh);
    void setRst(bool rst);
    void setSyn(bool syn);
    void setFin(bool fin);
    void setWinSize(u_int16_t win_size);
};

class CUDP
{
private:
    u_int16_t   src_port;
    u_int16_t   dst_port;
public:
    CUDP();
    ~CUDP();
    CUDP(u_int16_t src_port, u_int16_t dst_port);
    CUDP(const CUDP &ref);
    CUDP &operator=(const CUDP &ref);
    inline u_int16_t getSrcPort(void) { return src_port; }
    inline u_int16_t getDstPort(void) { return dst_port; }
    void setSrcPort(u_int16_t src_port);
    void setDstPort(u_int16_t dst_port);
};