#include "Count.h"

CCount::CCount(u_int32_t sig_id, u_int8_t rev, int limit, time_t timeout)
    : sig_id(sig_id), rev(rev), limit(limit), timeout(timeout)
{
}

CCount::~CCount()
{
}

CCount &CCount::operator=(const CCount &ref)
{
    rev = ref.rev;
    sig_id = ref.sig_id;

    timeout = ref.timeout;
    limit = ref.limit;

    timeInfo = ref.timeInfo;
    packet = ref.packet;

    return *this;
}

CCount::CCount(const CCount &ref)
    : sig_id(ref.sig_id), rev(ref.rev), limit(ref.limit), timeout(ref.timeout)
{
    this->timeInfo = ref.timeInfo;
    this->packet = ref.packet;
}

bool CCount::isMatched()
{
    if (timeInfo.size() >= limit)
    {
        return true;
    }
    else
    {
        logged = false;
        return false;
    }
}

void CCount::insertPacket(CPacket &packet)
{
    this->timeInfo.push_back(packet.time);
    this->packet.push_back(packet);
}

void CCount::deleteTimeOutPacket(void) //반드시 삽입후에 호출
{
    while ((timeInfo.back() - timeInfo.front()) > timeout)
    {
        timeInfo.erase(timeInfo.begin());
        packet.erase(packet.begin());
    }
}

void CCount::logging(CDB *db)
{
    db->logging(packet.back(), sig_id);
}