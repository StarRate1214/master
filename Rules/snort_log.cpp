#include "../HEADER/Rule_header.h"
#include <fstream>
#include <time.h>
#include <cstring>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define LOCATION "./LogData/"

const std::string currentDateTime() 
{
    time_t     now = time(0); //현재 시간을 time_t 타입으로 저장
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y_%m_%d.log", &tstruct); // YYYY-MM-DD 형태의 스트링
    const std::string day(buf);
    return buf;
}

int main() 
{
    if(access(LOCATION,0))
        if(0!=mkdir(LOCATION, 0754)) return -1;
    
    std::ofstream   logfile;
    std::string loc_set = LOCATION+currentDateTime();

    char loc[loc_set.size()+1];

    std::strcpy(loc, loc_set.c_str());
    logfile.open(loc, std::ios::out | std::ios::app);
    if (!logfile) return 1;

    logfile <<"hi"<<std::endl;//here is log data

    logfile.close();
    return 0;
}