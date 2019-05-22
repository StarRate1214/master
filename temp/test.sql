use test;

INSERT INTO signature  VALUES( 
    1,#DB에서 룰 관리용 번호
    "ASDF",#룰 이름
    1,#룰 버전
    1,#룰 고유번호
    1,#룰 그룹 번호
    "alert udp 192.168.0.0/24 any -> !192.168.0.0/24 any",#룰 헤더
    "(msg:"in-out udp"; sid: 1000021;)"#룰 옵션, general rule option은 제거
);

#SELECT * FROM event;
#SELECT * FROM iphdr;
#SELECT * FROM signature;