#include "Charstr.h"

std::vector<int> CCharstr::getPi(const char * patt, const int pSize)
{
    int j = 0;
	std::vector<int> pi(pSize, 0);

	for (int i = 1; i < pSize; i++)
	{
		while (j > 0 && patt[i] != patt[j])
			j = pi[j - 1];
		if (patt[i] == patt[j])
			pi[i] = ++j;
	}
	return pi;
}
int CCharstr::isSame(const char * text, const int tSize, const char * patt, const char pSize)
{
    int a = 0;
	if (tSize != pSize)
		return 0;
	for (int i = 0; i < tSize; i++)
		if (text[i] == patt[i])
			a++;

	if (tSize == a)
		return 1;
	else
		return 0;
}
int CCharstr::search(const char * text, const int tSize, const char patt, int start=0)
{
    if (tSize < start || tSize == 0)
			return -1;

	for (int i = start; i < tSize; i++)
		if (text[i] == patt)
			return i;

	return -1;
}
int CCharstr::search(const char * text, const int tSize, const char * patt, const int pSize, int start = 0)
{
    if (tSize < start || tSize == 0 || pSize == 0 || tSize < pSize)
	return -1;


    std::vector<int> failure = getPi(patt, pSize);

    int i = start;
    int j = 0;
    int pos = -1;
    while (i < tSize)
    {
    	if (text[i] == patt[j])
    	{
    		if (j == pSize - 1)
    			return i - pSize + 1;
    		i++;
    		j++;
    	}
    	else if (j == 0) i++;
    	else j = failure[j - 1];
    }
    return -1;

}
void substr(char * text, int * tSize, int start, int size=0)
{
    int i;
	if (size == 0)
		*tSize = *tSize - start;
	else
	{
		if (*tSize < start + size)
			*tSize = *tSize - start;
		else
			*tSize = size;
	}

	for (i = 0; i < *tSize; i++)
		text[i] = text[start + i];
}
void substrto(char * dest, int * dSize, const char * text, const int tSize, int start, int size=0)
{
    int i;
	if (size == 0)
		*dSize = tSize - start;

	else
	{
		if (tSize < start + size)
			*dSize = tSize - start;
		else
			*dSize = size;
	}

	for (i = 0; i < *dSize; i++)
		dest[i] = text[start + i];
}