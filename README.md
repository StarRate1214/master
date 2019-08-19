# master
Observer v0.2
ICMP
    PING : icmp-type 8
    Timestamp Request : icmp-type 13
    Information : icmp-type 15
    Address Mask Request : icmp-type 17

Scan
    XMAS : nmap -sX [attack-ip]
    Half-Open Scan :
    Fin Scan : nmap -sF [attack-ip]
    //Nmap -sF : nmap -sF [attack-ip]

Brute_Force
    SSH
    FTP(permisson, Detect)
    Telnet(failed, incorrect)

NetBus
    CaptureScreen
    GetApps
    Keylogging

Spoofing
    DNS
    IP(not yet)

sql_injection
    script 
    '
    "
    or
    and
    union
    having
    order by













Land Attack
	공격
	hping3 --icmp [피해자ip] -d [데이터크기] --flood -a [피해자ip]
	
	룰(탐지)
	alert tcp any any -> any(또는 서버ip대역) any (msg:"Land Attack";sameip;)

Ping of Death
	공격
	hping3 --icmp [피해자ip] -d [데이터크기] --flood

	룰(탐지)
	alert icmp any any -> any(또는 서버ip대역) any (msg:"Ping of Death"; detection_filter:track by_src, count 20, seconds 10;)

Syn Port Scan
	공격
	nmap -sS [피해자ip]

	룰(탐지)
	alert tcp [피해자 광대역 라우터] any -> [피해자ip] any (msg:"Syn Port Scan"; detection_filter:track by_src, count 5, seconds 10;)

Syn Flood Attack
	공격
	hping3 -syn -S [피해자ip] --flood -rand-source

	룰(탐지)
	alert tcp any any -> [피해자ip] any (flags:S msg:"Syn Flood Attack"; detection_filter:track by_dst, count 3, seconds 1;)

FTP Bounce Attack
	공격
	https://blog.naver.com/cuc00/20003550815 참고

	사전대책
	Anonymous FTP일 경우 ./incoming 에 퍼미션을 555로 준다.

	룰(탐지)
	alert tcp [피해자ip] 1024 -> any any (msg:"FTP Bounce Attack";  detection_filter:track by_src, count 3, seconds 1;)
	







