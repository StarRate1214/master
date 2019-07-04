#include "Count.h"

CCount::CCount(u_int32_t sig_id, u_int8_t rev, int limit, time_t timeout)
    :sig_id(sig_id), rev(rev), limit(limit), timeout(timeout)
{
    
}

CCount &CCount::operator=(CCount &ref)
{
    this->rev = ref.rev;
    this->sig_id = ref.sig_id;

    this->timeout = ref.timeout;
    this->limit = ref.limit;

    this->timeInfo = ref.timeInfo;
    this->packet = ref.packet;
}

CCount::CCount(CCount &ref)
    :sig_id(ref.sig_id), rev(ref.rev), limit(ref.limit), timeout(ref.timeout)
{
    this->timeInfo = ref.timeInfo;
    this->packet = ref.packet;
}

int CCount::getPacketCount(void)
{
    return packet.size();
}

void CCount::insertPacket(CPacket& packet)
{
    this->timeInfo.push_back(packet.time);
    this->packet.push_back(packet);
}