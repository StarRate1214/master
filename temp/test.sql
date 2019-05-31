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

INSERT INTO signature(sig_msg,sig_rev,sig_sid,sig_gid,sig_rule_header,sig_rule_option) VALUES( 
    'test_rule_content:"http"; nocase;',#룰 이름
    1,#룰 버전
    100,#룰 고유번호
    1,#룰 그룹 번호
    "alert tcp any any -> any 80",#룰 헤더
    'content:"http"; nocase;'#룰 옵션, general rule option은 제거
);

INSERT INTO signature(sig_msg,sig_rev,sig_sid,sig_gid,sig_rule_header,sig_rule_option) VALUES( 
    "test_rule_content:!"asdf"; http_cookie;",#룰 이름
    1,#룰 버전
    101,#룰 고유번호
    1,#룰 그룹 번호
    "alert tcp any any -> any 80",#룰 헤더
    'content:!"asdf"; http_cookie; '#룰 옵션, general rule option은 제거
);

INSERT INTO signature(sig_msg,sig_rev,sig_sid,sig_gid,sig_rule_header,sig_rule_option) VALUES( 
    'test_rule_content:"|48 54 54 50|";',#룰 이름
    1,#룰 버전
    102,#룰 고유번호
    1,#룰 그룹 번호
    "alert tcp any any -> any 80",#룰 헤더
    'content:"|48 54 54 50|";'#룰 옵션, general rule option은 제거
);

INSERT INTO signature(sig_msg,sig_rev,sig_sid,sig_gid,sig_rule_header,sig_rule_option) VALUES( 
    'test_rule_content:"|48 54 54 50|";',#룰 이름
    1,#룰 버전
    102,#룰 고유번호
    1,#룰 그룹 번호
    "alert tcp any any -> any 80",#룰 헤더
    'content:"http"; pcre:"aaa/i";'#룰 옵션, general rule option은 제거
);

#SELECT * FROM event;
#SELECT * FROM iphdr;
#SELECT * FROM signature;
