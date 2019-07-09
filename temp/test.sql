use test;
#SELECT * FROM event;
#SELECT * FROM iphdr;

DELETE FROM signature;

INSERT INTO signature(sig_msg,sig_rev,sig_sid,sig_gid,sig_rule_header,sig_rule_option) VALUES( 
    'test_rule_content:"http"; nocase;',#룰 이름
    1,#룰 버전
    100,#룰 고유번호
    1,#룰 그룹 번호
    "alert icmp any any -> any any",#룰 헤더
    'count:3/5;'#룰 옵션, general rule option은 제거
);

SELECT * FROM signature;