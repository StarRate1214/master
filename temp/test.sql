use test;

INSERT INTO signature(sig_msg,sig_rev,sig_sid,sig_gid,sig_rule_header,sig_rule_option) VALUES( 
    "asd",#룰 이름
    2,#룰 버전
    2,#룰 고유번호
    2,#룰 그룹 번호
    "alert udp $123 any -> !$33 any",#룰 헤더
    "( msg:'in-out udp'; sid: 1000021; )"#룰 옵션, general rule option은 제거
);

#SELECT * FROM event;
#SELECT * FROM iphdr;
#SELECT * FROM signature;