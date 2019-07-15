use test;
#SELECT * FROM event;
#SELECT * FROM iphdr;

DELETE FROM signature;


INSERT INTO signature(sig_msg,sig_rev,sig_sid,sig_gid,sig_rule_header,sig_rule_option) VALUES( 
    'country KR',#룰 이름
    1,#룰 버전
    33,#룰 고유번호
    1,#룰 그룹 번호
    "alert tcp any any <> any any",#룰 헤더
    'content:"HTTP"; nocase; content:"02-578-5551"; nation:KR;'#룰 옵션, general rule option은 제거
);
#INSERT INTO signature(sig_msg,sig_rev,sig_sid,sig_gid,sig_rule_header,sig_rule_option) VALUES( 
#    'country JP',#룰 이름
#    1,#룰 버전
#    334,#룰 고유번호
#    1,#룰 그룹 번호
#    "alert tcp any any <> any any",#룰 헤더
#    'nation:JP;'#룰 옵션, general rule option은 제거
#);
#INSERT INTO signature(sig_msg,sig_rev,sig_sid,sig_gid,sig_rule_header,sig_rule_option) VALUES( 
#    'country CN',#룰 이름
#    1,#룰 버전
#    453,#룰 고유번호
#    1,#룰 그룹 번호
#    "alert tcp any any <> any any",#룰 헤더
#    'nation:CN;'#룰 옵션, general rule option은 제거
#);
#INSERT INTO signature(sig_msg,sig_rev,sig_sid,sig_gid,sig_rule_header,sig_rule_option) VALUES( 
#    'country HK',#룰 이름
#    1,#룰 버전
#    567,#룰 고유번호
#    1,#룰 그룹 번호
#    "alert tcp any any <> any any",#룰 헤더
#    'nation:HK;'#룰 옵션, general rule option은 제거
#);
INSERT INTO signature(sig_msg,sig_rev,sig_sid,sig_gid,sig_rule_header,sig_rule_option) VALUES( 
    'country US',#룰 이름
    1,#룰 버전
    789,#룰 고유번호
    1,#룰 그룹 번호
    "alert tcp any any <> any any",#룰 헤더
    'content:"GMT"; nation:US;'#룰 옵션, general rule option은 제거
);

SELECT * FROM signature;
