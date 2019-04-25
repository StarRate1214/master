//snort head action log
#include "../HEADER/Rule_header.h"
#include <fstream>
#include <time.h>
#include <cstring>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

const std::string currentDate() //일별 파일 구분을 위한 시간함수
{
    time_t now = time(0);                                 //현재 시간을 time_t 타입으로 저장
    struct tm tstruct;                                    //시간 구조체
    char buf[80];                                         //시간을 담을 버퍼
    tstruct = *localtime(&now);                           //컴퓨터 시간 받아옴
    strftime(buf, sizeof(buf), "%Y_%m_%d.log", &tstruct); // YYYY-MM-DD 형태의 스트링
    return buf;
}
const std::string currentTime() //로그를 남길때를 위한 타임스템프
{
    time_t now = time(0);                                       //현재 시간을 time_t 타입으로 저장
    struct tm tstruct;                                          //시간 구조체
    char buf[80];                                               //시간을 담을 버퍼
    tstruct = *localtime(&now);                                 //현재 컴퓨터 시간을 받아옴
    strftime(buf, sizeof(buf), "%y/%m/%d-%X.ffffff", &tstruct); // YYYY-MM-DD 형태의 스트링
    return buf;
}
int SnortLog(char * location = "./LogData/") //기본을 ./LogData/ 하위에 만듦
{
    if (access(location, 0)) //디렉토리 존재 여부를 확인
        if (0 != mkdir(location, 4754))
            return -1; //오류 표현

    std::ofstream logfile;                          //파일 쓰기형식으로 열기
    std::string loc_set = location + currentDate(); // 전체 위치

    char loc[loc_set.size() + 1]; //string 을 char 형으로 변환

    std::strcpy(loc, loc_set.c_str());                // 복사
    logfile.open(loc, std::ios::out | std::ios::app); //일별로 log파일이 존재하면 끝에 쓰고 아니면 새로만듦
    if (!logfile)
        return -1; //에러발생시 -1 리턴

    logfile << currentTime() << std::endl; //here is log data

    logfile.close(); //파일 close
    return 0;
}