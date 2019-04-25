#pragma once
#include "Rule_header.h"
#include "TCP_UDP.h"
#include <fstream>
#include <time.h>
#include <cstring>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

const std::string currentDate(); // .log 파일의 이름을 지정해주는 함수 년,월,일을 반환
const std::string currentTime(); // .log 파일에 기록을 남길때 사용할 타임스탬프를 생성
int SnortLog(const char *location); // defult는 하위에 LogData 디렉터리를 생성함 또는 경로를 지정해 만들수 있음. 리턴은 정상적일땐 0 실패 또는 에러시 -1