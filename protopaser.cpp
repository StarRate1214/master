
#include <ids.h>
#include <string>
#include <iostream>
#include <boost/algorithm/string.hpp>

typedef struct protopaser
{
    int order;
    int sig_id;
    std::string header;
    std::string option;
} sPROTOCOL;

sPROTOCOL Sliceham(char *proto)
{
    //INSERT sig_id=777, header=alert tcp any any -> any any, option=content:\"HTTP\";
    //DELETE sig_id=1\n
    sPROTOCOL ret;
    char *ret_ptr;
    char *next_ptr;
    char *value;
    char *value1;
    char *value2;
    char *str = strtok_r(proto, " ", &next_ptr);
    //INSERT sig_id=777, header=alert tcp any any -> any any, option=content:\"HT,TP\";";
    if (str[0] == 'I') //INSERT
    {
        ret.order = INSERT;

        ret_ptr = strtok_r(NULL, ",", &next_ptr);
        strtok_r(ret_ptr, "=", &value);
        ret.sig_id = atoi(value);

        ret_ptr = strtok_r(NULL, ",", &next_ptr);
        strtok_r(ret_ptr, "=", &value);
        ret.header = value;

        strtok_r(NULL, "=", &next_ptr);
        ret.option = next_ptr;

        std::cout << ret.order << std::endl; 
        std::cout << ret.sig_id << std::endl;
        std::cout << ret.header << std::endl;
        std::cout << ret.option << std::endl;
    }
    else if (str[0] == 'U') //UPDATE
    {
        ret.order = UPDATE;
    }
    else if (str[0] == 'D') //DELETE
    {
        ret.order = DELETE;
    }

    return ret;
}

int main()
{
    char proto[1024] = "INSERT sig_id=777, header=alert tcp any any -> any any, option=content:\"HT,TP\";";

    sPROTOCOL ret = Sliceham(proto);

    return 0;
}