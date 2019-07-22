use test;
#SELECT * FROM event;
#SELECT * FROM iphdr;

DELETE FROM signature;

INSERT INTO signature(sig_msg,sig_rev,sig_sid,sig_gid,  sig_action, sig_protocol,sig_srcIP,sig_srcPort, sig_direction,  sig_dstIP, sig_dstPort, sig_rule_option) VALUES( 
    'test',#룰 이름
    1,#룰 버전
    789,#룰 고유번호
    1,#룰 그룹 번호
    'alert',
    'icmp',
    'any',
    'any',
    '->',
    'any',
    'any',#룰 헤더
    ''#룰 옵션, general rule option은 제거
);

SELECT * FROM signature;
