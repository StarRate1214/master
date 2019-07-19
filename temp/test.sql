use test;
#SELECT * FROM event;
#SELECT * FROM iphdr;

DELETE FROM signature;

INSERT INTO signature(sig_msg,sig_rev,sig_sid,sig_gid,sig_rule_header,sig_rule_option) VALUES( 
    'test_rule_detection_filter',#룰 이름
    1,#룰 버전
    789,#룰 고유번호
    1,#룰 그룹 번호
    'alert tcp any any -> any any',#룰 헤더
    'content:"HTTP"; nation:KR; detection_filter:track by_src, count 2, seconds 2;'#룰 옵션, general rule option은 제거
);

INSERT INTO signature(sig_msg,sig_rev,sig_sid,sig_gid,sig_rule_header,sig_rule_option) VALUES( 
    'test_rule_KR',#룰 이름
    1,#룰 버전
    789,#룰 고유번호
    1,#룰 그룹 번호
    'alert tcp any any -> any any',#룰 헤더
    'content:"HTTP"; nation:US;'#룰 옵션, general rule option은 제거
);

SELECT * FROM signature;
