DROP DATABASE test;
CREATE DATABASE test;
use test;

#Group Table
CREATE TABLE sig_port_variables( 
    v_name VARCHAR(255),
    v_value VARCHAR(255),
    v_description VARCHAR(255)
);

#Group Table
CREATE TABLE sig_ip_variables( 
    v_name VARCHAR(255),
    v_value VARCHAR(255),
    v_description VARCHAR(255)
);

#Group Table
CREATE TABLE sig_group( 
    gid INT UNSIGNED NOT NULL AUTO_INCREMENT,
    gname VARCHAR(255),
    PRIMARY KEY (gid)
);

INSERT INTO sig_group VALUES( 
    1,#기본 그룹
    "DEFAULT"
);

INSERT INTO sig_port_variables(v_name,v_value) VALUES
("$FTP-Data(T)",20),
("$FTP(T)",21),
("$SSH(T)",22),
("$Telnet(T)",23),
("$SMTP(T)",25),
("$DNS(TU)",53),
("$HTTP(TU)",80),
("$Kerberos(T)",88),
("$POP3(T)",110),
("$RPC(TU)",111),
("$SFTP(T)",115),
("$SQL_Service(TU)",118),
("$NNTP(T)",119),
("$NTP(U)",123),
("$NetBIOS(T)",139),
("$SNMP-Agent(U)",161),
("$SNMP-Manager(U)",162),
("$IRC(U)",194),
("$IMAP3(TU)",220),
("$HTTPS(T)",443),
("$MS-DS(TU)",445),
("$SMTP(T)",587),
("$Doom(T)",666),
("$MS-sql(TU)",1433),
("$mysql(T)",3306);


#Rule Table
CREATE TABLE signature ( 
    sig_run BOOLEAN,
    sig_id  INT UNSIGNED NOT NULL   AUTO_INCREMENT,#DB에서 룰 관리용 번호
    sig_msg    VARCHAR(255)    NOT NULL,#룰 이름
    sig_rev TINYINT UNSIGNED,#룰 버전
    sig_sid INT UNSIGNED,#룰 고유번호
    sig_gid INT UNSIGNED,#룰 그룹 번호
    sig_action VARCHAR(255) NOT NULL,
    sig_protocol VARCHAR(255) NOT NULL,
    sig_srcIP VARCHAR(255) NOT NULL,
    sig_srcPort VARCHAR(255) NOT NULL,
    sig_direction CHAR(2) NOT NULL,
    sig_dstIP VARCHAR(255) NOT NULL,
    sig_dstPort VARCHAR(255) NOT NULL,
    sig_rule_option VARCHAR(1024),#룰 옵션, general rule option은 제거
    PRIMARY KEY (sig_id),
    FOREIGN KEY (sig_gid) REFERENCES sig_group (gid) 
        ON DELETE CASCADE
        ON UPDATE CASCADE
);

#Event Table
CREATE TABLE event  ( 
    eid INT UNSIGNED    NOT NULL    AUTO_INCREMENT,#로그 번호
    sig_id  INT UNSIGNED, #DB에서 룰 관리용 번호
    time   DATETIME    NOT NULL,#패킷 도착 시간
    true_rate   INT UNSIGNED,#정탐일 확률
    PRIMARY KEY (eid),
    FOREIGN KEY (sig_id) REFERENCES signature (sig_id) 
        ON DELETE SET NULL
        ON UPDATE CASCADE
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
        ON DELETE CASCADE
        ON UPDATE CASCADE
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
        ON DELETE CASCADE
        ON UPDATE CASCADE
);

#UDP Table
CREATE TABLE udphdr(
    eid INT UNSIGNED    NOT NULL,#로그 번호
    src_port    SMALLINT    UNSIGNED    NOT NULL,
    dst_port    SMALLINT    UNSIGNED    NOT NULL,
    PRIMARY KEY (eid),
    FOREIGN KEY (eid) REFERENCES event (eid)
        ON DELETE CASCADE
        ON UPDATE CASCADE
);

#ICMP Table
CREATE TABLE icmphdr( 
    eid INT UNSIGNED    NOT NULL,#로그 번호
    type   TINYINT  UNSIGNED NOT NULL,
    code   TINYINT  UNSIGNED NOT NULL,
    PRIMARY KEY (eid),
    FOREIGN KEY (eid) REFERENCES event (eid)
    ON DELETE CASCADE
    ON UPDATE CASCADE
);

#Data Payload Table
CREATE TABLE data   (
    eid INT UNSIGNED    NOT NULL,#로그 번호
    data_payload  BLOB,
    PRIMARY KEY (eid),
    FOREIGN KEY (eid) REFERENCES event (eid)
    ON DELETE CASCADE
    ON UPDATE CASCADE
);
 
CREATE VIEW base_view AS
    select event_view.eid, time,sig_msg, src_ip, dst_ip, sig_protocol, sig_id
    from (select eid, time, sig_protocol, sig_msg, event.sig_id from event, signature where event.sig_id=signature.sig_id) as event_view, iphdr 
    where iphdr.eid=event_view.eid;

CREATE VIEW event_view AS
(select base_view.eid, time,sig_msg, src_ip, src_port, dst_ip, dst_port, sig_protocol, sig_id from base_view, tcphdr where base_view.eid=tcphdr.eid)
union
(select base_view.eid, time,sig_msg, src_ip, src_port, dst_ip, dst_port, sig_protocol, sig_id from base_view, udphdr where base_view.eid=udphdr.eid)
union 
(select eid, time, sig_msg, src_ip, null as src_port, dst_ip, null as dst_port, sig_protocol , sig_id from base_view where sig_protocol='icmp')
order by eid;