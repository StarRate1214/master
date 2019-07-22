#pragma once
#include "ids.h"

class CCharstr
{
private:
    std::vector<int> getPi(const char * patt, const int pSize);
public:
    inline char at(const char* text, const int pos) { return text[pos]; }
    int isSame(const char * text, const int tSize, const char * patt, const char pSize);
    int search(const char * text, const int tSize, const char patt, int start);
    int search(const char * text, const int tSize, const char * patt, const int pSize, int start);
    void substr(char * text, int * tSize, int start, int size);
    void substrto(char * dest, int * dSize, const char * text, const int tSize, int start, int size);
};