use test;

#Rule Table
CREATE TABLE signature ( 
    sig_id  INT UNSIGNED NOT NULL   AUTO_INCREMENT,#DB에서 룰 관리용 번호
    sig_msg    VARCHAR(255)    NOT NULL,#룰 이름
    sig_rev TINYINT UNSIGNED,#룰 버전
    sig_sid INT UNSIGNED,#룰 고유번호
    sig_gid INT UNSIGNED,#룰 그룹 번호
    sig_rule_header VARCHAR(255) NOT NULL,#룰 헤더
    sig_rule_option VARCHAR(255),#룰 옵션, general rule option은 제거
    PRIMARY KEY (sig_id)
);

#Event Table
CREATE TABLE event  ( 
    eid INT UNSIGNED    NOT NULL    AUTO_INCREMENT,#로그 번호
    sig_id  INT UNSIGNED    NOT NULL, #DB에서 룰 관리용 번호
    time   DATETIME    NOT NULL,#패킷 도착 시간
    true_rate   INT UNSIGNED,#정탐일 확률
    PRIMARY KEY (eid),
    FOREIGN KEY (sig_id) REFERENCES signature (sig_id)
);

#IP Table
CREATE TABLE iphdr  (
    eid INT UNSIGNED    NOT NULL,#로그 번호
    src_ip  INT UNSIGNED    NOT NULL,
    dst_ip  INT UNSIGNED    NOT NULL,
    tos  TINYINT UNSIGNED,
    ttl  TINYINT UNSIGNED,
    more_frag   BOOLEAN,
    dont_frag   BOOLEAN,
    PRIMARY KEY (eid),
    FOREIGN KEY (eid) REFERENCES event (eid)
);

#TCP Table
CREATE TABLE tcphdr(  
    eid INT UNSIGNED    NOT NULL,#로그 번호
    src_port    SMALLINT    UNSIGNED    NOT NULL,
    dst_port    SMALLINT    UNSIGNED    NOT NULL,
    seq_num INT UNSIGNED,
    ack_num INT UNSIGNED,
    urg BOOLEAN,
    ack BOOLEAN,
    psh BOOLEAN,
    rst BOOLEAN,
    syn BOOLEAN,
    fin BOOLEAN,
    win_size    SMALLINT    UNSIGNED,
    PRIMARY KEY (eid),
    FOREIGN KEY (eid) REFERENCES event (eid)
);

#UDP Table
CREATE TABLE udphdr(
    eid INT UNSIGNED    NOT NULL,#로그 번호
    src_port    SMALLINT    UNSIGNED    NOT NULL,
    dst_port    SMALLINT    UNSIGNED    NOT NULL,
    PRIMARY KEY (eid),
    FOREIGN KEY (eid) REFERENCES event (eid)
);

#ICMP Table
CREATE TABLE icmphdr( 
    eid INT UNSIGNED    NOT NULL,#로그 번호
    type   TINYINT  UNSIGNED NOT NULL,
    code   TINYINT  UNSIGNED NOT NULL,
    PRIMARY KEY (eid),
    FOREIGN KEY (eid) REFERENCES event (eid)
);

#Data Payload Table
CREATE TABLE data   (
    eid INT UNSIGNED    NOT NULL,#로그 번호
    data_payload  TEXT,
    PRIMARY KEY (eid),
    FOREIGN KEY (eid) REFERENCES event (eid)
);