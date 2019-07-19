#include "RuleEngine.h"
#include <pcrecpp.h>
//https://www.joinc.co.kr/w/Site/C++/pcrecpp
//https://www.pcre.org/original/doc/html/pcrecpp.html
//g++ -o pcrecpptest pcrecpptest.cc -lpcrecpp
//메타문자 : ^ . $ | ( ) [ ] * + ? \ /

bool CRuleEngine::pcre(std::string pcre)	//pcre = 정규표현식
{		
	char* rawpacket = (char*)packet.data_payload;	//비교할 패킷데이터
	std::string tmp="";								//매칭된 문자열 저장
	std::string pcre_flag="";							//pcre에서 flag 옵션 저장
	pcrecpp::RE_Options flag;						//pcre_flag에서 나온 옵션을 RE_Options형태로 저장
	//std::cout <<"pcre origin"<<pcre<<std::endl;
	//pcre flag 찾는부분
	int a = pcre.rfind('/')+1;
	int b = pcre.rfind('"')-a;
	pcre_flag = pcre.substr(a, b);
	//std::cout << "pcre flag:" <<pcre_flag << " " << a << " " << b << std::endl;
	
	//쌍따옴표 제거부분
	pcre = pcre.substr(1, pcre.size()-2);
	//std::cout << "pcre:" << pcre << std::endl;

	//flag를 뺀 순수한 pcre
	pcre = pcre.substr(1, pcre.rfind('/')-1);
	//std::cout << "pure at PCRE : " << pcre << std::endl;
	
	//flag option 설정 하기
	const char* temp_flag = pcre_flag.c_str();
	for(int i=0; i<pcre_flag.size(); i++)
	{
		switch(temp_flag[i])
		{
			case 'i':
				flag.set_caseless(true);	//i option
				break;
			case 's':
				flag.set_dotall(true);		//s option
				break;
			case 'm':
				flag.set_multiline(true);	//m option
				break;
			case 'x':
				flag.set_extended(true);	//x option
				break;
		}
	}
	//std::cout << "flag : " << flag.all_options() <<std::endl;
	//std::cout << std::endl;

	//맨앞과 맨뒤에 () 추가
	pcre.insert(0,"(");
	pcre.insert(pcre.size(),")");
	
	//pcre와 flag를 사용해서 pcre_set 설정
	pcrecpp::RE pcre_set(pcre, flag);

	//pcre_set과 정제된packet을 정규표현식 부분매칭
	bool ret;

	ret = pcre_set.PartialMatch(rawpacket, &tmp);
	if(ret==true)
		return ret;//찾으면1 못찾으면0

	for(int i=0;i<packet.data_payload_size;i++)
	{
		if(rawpacket[i]=='\0' && i!=packet.data_payload_size-1)
		{
			ret = pcre_set.PartialMatch(rawpacket+i+1, &tmp);
			if(ret==true)
				return ret;//찾으면1 못찾으면0
		}
	}
	//std::cout << "Matched String : " << tmp << std::endl;
	return ret;
}
