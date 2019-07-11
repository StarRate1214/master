#include "detection_filter.h"

CDetection_filter::CDetection_filter(u_int32_t sig_id, u_int8_t rev, int limit, time_t timeout, u_int32_t ip_addr)
    : sig_id(sig_id), rev(rev), limit(limit), timeout(timeout), ip_addr(ip_addr)
{
}

CDetection_filter::~CDetection_filter()
{
}

CDetection_filter &CDetection_filter::operator=(const CDetection_filter &ref)
{
    rev = ref.rev;
    sig_id = ref.sig_id;

    timeout = ref.timeout;
    limit = ref.limit;
    ip_addr = ref.ip_addr;

    timeInfo = ref.timeInfo;
    packet = ref.packet;


    return *this;
}

CDetection_filter::CDetection_filter(const CDetection_filter &ref)
    : sig_id(ref.sig_id), rev(ref.rev), limit(ref.limit), timeout(ref.timeout), ip_addr(ref.ip_addr)
{
    this->timeInfo = ref.timeInfo;
    this->packet = ref.packet;
}

bool CDetection_filter::isMatched()
{
    if (timeInfo.size() >= limit)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void CDetection_filter::insertPacket(CPacket &packet)
{
    this->timeInfo.push_back(packet.time);
    this->packet.push_back(packet);
}

void CDetection_filter::deleteTimeOutPacket(void) //반드시 삽입후에 호출
{
    while ((timeInfo.back() - timeInfo.front()) > timeout)
    {
        timeInfo.erase(timeInfo.begin());
        packet.erase(packet.begin());
    }
}