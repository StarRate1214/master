#include "Count.h"

void CCount::deleteTimeOutPacket(time_t current_time)//반드시 삽입후에 호출
{
    while((timeInfo.back()-timeInfo.front())>timeout)
    {
        timeInfo.erase(timeInfo.begin());
        packet.erase(timeInfo.begin());
    }
}

void CCount::logging(CDB *db)
{
    if(logged)   
    {
        db->logging(packet.back(), sig_id);
    }
    else
    {
        for (size_t i = 0; i < timeInfo.size(); i++)
        {
            db->logging( packet.at(i),sig_id);
        }
        logged=true;
    }
}