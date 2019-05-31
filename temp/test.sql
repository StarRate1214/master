use test;
#SELECT * FROM event;
#SELECT * FROM iphdr;
#SELECT * FROM signature;

INSERT INTO signature(sig_msg,sig_rev,sig_sid,sig_gid,sig_rule_header,sig_rule_option) VALUES( 
    "test_rule_content:attack",#룰 이름
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

INSERT INTO signature(sig_msg,sig_rev,sig_sid,sig_gid,sig_rule_header,sig_rule_option) VALUES( 
    "SQL Injection Detected",#룰 이름
    1,#룰 버전
    100000001,#룰 고유번호
    1,#룰 그룹 번호
    "alert tcp any any -> any 80",#룰 헤더
    'content: "|27|" ;'#룰 옵션, general rule option은 제거
);

INSERT INTO signature(sig_msg,sig_rev,sig_sid,sig_gid,sig_rule_header,sig_rule_option) VALUES( 
    "SQL Injection Detected",#룰 이름
    1,#룰 버전
    100000002,#룰 고유번호
    1,#룰 그룹 번호
    "alert tcp any any -> any 80",#룰 헤더
    'content: "|22|" ;'#룰 옵션, general rule option은 제거
);

INSERT INTO signature(sig_msg,sig_rev,sig_sid,sig_gid,sig_rule_header,sig_rule_option) VALUES( 
    "AND SQL Injection Detected",#룰 이름
    1,#룰 버전
    100000003,#룰 고유번호
    1,#룰 그룹 번호
    "alert tcp any any -> any 80",#룰 헤더
    'content: "and" ; nocase ;'#룰 옵션, general rule option은 제거
);

INSERT INTO signature(sig_msg,sig_rev,sig_sid,sig_gid,sig_rule_header,sig_rule_option) VALUES( 
    "OR SQL Injection Detected",#룰 이름
    1,#룰 버전
    100000004,#룰 고유번호
    1,#룰 그룹 번호
    "alert tcp any any -> any 80",#룰 헤더
    'content: "or" ; nocase ;'#룰 옵션, general rule option은 제거
);

INSERT INTO signature(sig_msg,sig_rev,sig_sid,sig_gid,sig_rule_header,sig_rule_option) VALUES( 
    "Order by SQL Injection",#룰 이름
    1,#룰 버전
    100000005,#룰 고유번호
    1,#룰 그룹 번호
    "alert tcp any any -> any 80",#룰 헤더
    'content: "order" ;'#룰 옵션, general rule option은 제거
);

use test;
INSERT INTO signature(sig_msg,sig_rev,sig_sid,sig_gid,sig_rule_header,sig_rule_option) VALUES( 
    "UNION SELECT SQL Injection",#룰 이름
    1,#룰 버전
    1000000006#룰 고유번호
    1,#룰 그룹 번호
    "alert tcp any any -> any 80",#룰 헤더
    'content: "union" ;'#룰 옵션, general rule option은 제거
);

INSERT INTO signature(sig_msg,sig_rev,sig_sid,sig_gid,sig_rule_header,sig_rule_option) VALUES( 
    "XSS PCRE Detected",#룰 이름
    1,#룰 버전
    1000000007#룰 고유번호
    1,#룰 그룹 번호
    "alert tcp any any -> any 80",#룰 헤더
    'pcre:"/(\%27)|(\)'#룰 옵션, general rule option은 제거
);

INSERT INTO signature(sig_msg,sig_rev,sig_sid,sig_gid,sig_rule_header,sig_rule_option) VALUES( 
    "TTL Over 64",#룰 이름
    1,#룰 버전
    1000000008#룰 고유번호
    1,#룰 그룹 번호
    "alert tcp 192.168.0.0/24 any -> any 80",#룰 헤더
    'ttl:>=64;'#룰 옵션, general rule option은 제거
);

NSERT INTO signature(sig_msg,sig_rev,sig_sid,sig_gid,sig_rule_header,sig_rule_option) VALUES( 
    "Des Port <=ssh",#룰 이름
    1,#룰 버전
    1000000009#룰 고유번호
    1,#룰 그룹 번호
    "alert tcp any :22 <> any any",#룰 헤더
    ''#룰 옵션, general rule option은 제거
);

NSERT INTO signature(sig_msg,sig_rev,sig_sid,sig_gid,sig_rule_header,sig_rule_option) VALUES( 
    "Same IP",#룰 이름
    1,#룰 버전
    1000000010#룰 고유번호
    1,#룰 그룹 번호
    "alert tcp any any -> any any",#룰 헤더
    'sameip;'#룰 옵션, general rule option은 제거
);

NSERT INTO signature(sig_msg,sig_rev,sig_sid,sig_gid,sig_rule_header,sig_rule_option) VALUES( 
    "ANY ICMP Detected",#룰 이름
    1,#룰 버전
    1000000011#룰 고유번호
    1,#룰 그룹 번호
    "alert icmp any any -> any any ",#룰 헤더
    NULL#룰 옵션, general rule option은 제거
);

NSERT INTO signature(sig_msg,sig_rev,sig_sid,sig_gid,sig_rule_header,sig_rule_option) VALUES( 
    "UDP",#룰 이름
    1,#룰 버전
    1000000012#룰 고유번호
    1,#룰 그룹 번호
    "alert udp 192.168.0.33 any -> 192.168.0.8 any",#룰 헤더
    NULL#룰 옵션, general rule option은 제거
);

#SELECT * FROM event;
#SELECT * FROM iphdr;
#SELECT * FROM signature;
