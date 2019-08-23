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
    severity TINYINT UNSIGNED,
    PRIMARY KEY (sig_id),
    FOREIGN KEY (sig_gid) REFERENCES sig_group (gid) 
        ON DELETE CASCADE
        ON UPDATE CASCADE
);

INSERT INTO `sig_group` VALUES (2,'ICMP'),(3,'Brute_Force'),(4,'NetBus'),(5,'FTP'),(7,'sql_injection'),(8,'SCAN');
INSERT INTO `signature` VALUES (1,21,'Timestamp Request',2,2,2,'alert','icmp','any','any','->','any','any','itype:13; ',2),(1,22,'Information',2,3,2,'alert','icmp','any','any','->','any','any','itype:15; ',2),(1,23,'Address Mask Request',2,4,2,'alert','icmp','any','any','->','any','any','itype:17; ',2),(1,24,'Half-Open Scan',7,4,8,'alert','tcp','any','any','->','$HOME','any','detection_filter:track by_all, count 20, seconds 6; flags:S; ',2),(1,25,'FIN_Scan',9,2,8,'alert','tcp','any','any','->','$HOME','any','flags:F; detection_filter:track by_all, count 20, seconds 6; ',2),(1,26,'XMAS',2,3,8,'alert','tcp','any','any','->','$HOME','any','flags:FPU; ',2),(1,28,'DNS spoofing',3,5,1,'alert','udp','any','$DNS(TU)','<>','any','any','content:\"|81 80 00 01 00 01 00 00 00 00|\"; content:\"|c0 0c 00 01 00 01 00 00 00 3c 00 04|\"; ',3),(1,33,'NetBus_Cpturer_Detect',4,3,4,'alert','tcp','any','any','->','$HOME','any','content:\"CaptureScreen\"; ',4),(0,34,'NetBus_GetApps_Detect',4,2,4,'alert','tcp','any','any','->','$HOME','any','content:\"GetApps\"; ',1),(1,35,'NetBus Keylogging Detect',5,1,4,'alert','tcp','any','any','->','$HOME','any','content:\"Listen\"; ',1),(1,37,'SSH Bruth Force',4,4,3,'alert','tcp','any','$SSH(T)','->','any','any','content:\"|53 53 48|\"; detection_filter:track by_src, count 20, seconds 10; ',3),(1,38,'FTP Brute Detect',11,3,3,'alert','tcp','any','$FTP(T)','->','any','any','content:\"530 login incorrect\"; nocase; ',3),(1,39,'Telnet Brute Force',5,2,3,'alert','tcp','any','$Telnet(T)','->','any','any','content:\"Login Failed\"; nocase; detection_filter:track by_src, count 3, seconds 10; ',1),(1,40,'telnet Brute force',5,1,3,'alert','tcp','any','$Telnet(T)','->','any','any','content:\"login incorrect\"; nocase; detection_filter:track by_src, count 3, seconds 10; ',3),(1,41,'Ftp permisson',3,5,3,'alert','tcp','any','$FTP(T)','->','any','any','content:\"503 permission denied\"; nocase; detection_filter:track by_src, count 4, seconds 6; ',1),(1,43,'NMAP -sF',1,1,8,'alert','tcp','any','any','->','$HOME','any','flagbits:!M; dsize:0; flags:FPU; ack:0; window:2048; detection_filter:track by_dst, count 1, seconds 60; ',2),(1,46,'XXE injection(]]>)',4,1,9,'alert','tcp','any','any','->','$HOME','$HTTP(TU)','content:\"]]>\"; ',3),(1,49,'XXE injection (/-->)',4,4,9,'alert','tcp','any','any','->','$HOME','$HTTP(TU)','content:\"/-->\"; ',3),(1,50,'XXE injection (-->)',2,5,9,'alert','tcp','any','any','->','$HOME','$HTTP(TU)','content:\"-->\"; ',3),(1,54,'XXE injection (<!)',2,6,9,'alert','tcp','any','any','->','$HOME','$HTTP(TU)','content:\"<!\"; ',3),(1,55,'XXE injection (<!--)',1,7,9,'alert','tcp','any','any','->','$HOME','$HTTP(TU)','content:\"<!--\"; ',3),(1,56,'XXE injection (<![CDATA[)',3,8,9,'alert','tcp','any','any','->','$HOME','$HTTP(TU)','content:\"<![CDATA[\"; ',3),(1,57,'XXE Dos',1,9,9,'alert','tcp','any','any','->','$HOME','$HTTP(TU)','content:\"<!ENTITY\"; detection_filter:track by_src, count 5, seconds 1; ',3),(1,58,'XXE injection (<!DOCTYPE)',2,10,9,'alert','tcp','any','any','->','$HOME','$HTTP(TU)','content:\"<!DOCTYPE\"; ',3),(1,59,'sql(having)',3,7,7,'alert','tcp','any','any','->','$HOME','$HTTP(TU)','content:\"having\"; nocase; ',2),(1,60,'sql(group by)',1,8,7,'alert','tcp','any','any','->','$HOME','$HTTP(TU)','content:\"group by\"; nocase; ',2),(1,61,'sql(union)',2,9,7,'alert','tcp','any','any','->','$HOME','$HTTP(TU)','content:\"union\"; nocase; content:\"select\"; nocase; ',2),(1,62,'sql(--)',3,10,7,'alert','tcp','any','any','->','$HOME','$HTTP(TU)','content:\"|27|\"; content:\"--\"; nocase; ',2),(1,63,'XSS(script l)',1,2,10,'alert','tcp','any','any','->','$HOME','$HTTP(TU)','content:\"<script\"; nocase; ',2),(1,64,'XSS(script r)',3,6,10,'alert','tcp','any','any','->','$HOME','$HTTP(TU)','content:\"script>\"; nocase; ',2),(1,65,'XSS(iframe l)',1,3,10,'alert','tcp','any','any','->','$HOME','$HTTP(TU)','content:\"<iframe\"; nocase; ',2),(1,66,'XSS(iframe r)',1,4,10,'alert','tcp','any','any','->','$HOME','$HTTP(TU)','content:\"iframe>\"; nocase; ',2),(1,67,'XSS(iframe src)',1,5,10,'alert','tcp','any','any','->','$HOME','$HTTP(TU)','content:\"iframe src\"; nocase; ',2),(1,68,'XSS(object l)',1,7,10,'alert','tcp','any','any','->','$HOME','$HTTP(TU)','content:\"<object\"; nocase; ',2),(1,69,'XSS(object r)',1,8,10,'alert','tcp','any','any','->','$HOME','$HTTP(TU)','content:\"object>\"; nocase; ',2),(1,70,'XSS(width=0)',1,9,10,'alert','tcp','any','any','->','$HOME','$HTTP(TU)','content:\"width=0\"; nocase; ',2),(1,71,'XSS(height=0)',1,10,10,'alert','tcp','any','any','->','$HOME','$HTTP(TU)','content:\"height=0\"; nocase; ',2),(1,72,'XSS(div style)',1,11,10,'alert','tcp','any','any','->','$HOME','$HTTP(TU)','content:\"div\"; nocase; content:\"style\"; nocase; ',2),(1,73,'BAC(admin)',3,1,11,'alert','tcp','any','any','->','$HOME','$HTTP(TU)','content:\"/admin\"; nocase; ',3),(1,74,'BAC(master)',1,2,11,'alert','tcp','any','any','->','$HOME','$HTTP(TU)','content:\"/master\"; nocase; ',3),(1,75,'BAC(ssi)',1,3,11,'alert','tcp','any','any','->','$HOME','$HTTP(TU)','content:\"<!--#\"; ',3),(1,76,'BAC(web admin attack)',4,4,11,'alert','tcp','any','any','->','$HOME','$HTTP(TU)','content:\"user_id=admin\"; nocase; detection_filter:track by_src, count 3, seconds 3; ',2),(1,77,'sql(version)',1,11,7,'alert','tcp','any','any','->','$HOME','$HTTP(TU)','content:\"@@version\"; nocase; ',3),(1,78,'BAC(Directory listing)',1,5,11,'alert','tcp','any','any','->','$HOME','$HTTP(TU)','content:\"index of /\"; nocase; ',3);

#Event Table
CREATE TABLE event  ( 
    eid INT UNSIGNED    NOT NULL    AUTO_INCREMENT,#로그 번호
    sig_id  INT UNSIGNED, #DB에서 룰 관리용 번호
    time   DATETIME    NOT NULL,#패킷 도착 시간
    true_rate   INT UNSIGNED,#정탐일 확률
    payload_size INT UNSIGNED,
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
    select event_view.eid, time,sig_msg, src_ip, dst_ip, sig_protocol, sig_id, true_rate, payload_size, severity
    from (select eid, time, sig_protocol, sig_msg, event.sig_id, true_rate, payload_size, severity from event, signature where event.sig_id=signature.sig_id) as event_view, iphdr 
    where iphdr.eid=event_view.eid;

CREATE VIEW event_view AS
(select base_view.eid, time,sig_msg, src_ip, src_port, dst_ip, dst_port, sig_protocol, sig_id, true_rate, payload_size, severity from base_view, tcphdr where base_view.eid=tcphdr.eid)
union
(select base_view.eid, time,sig_msg, src_ip, src_port, dst_ip, dst_port, sig_protocol, sig_id, true_rate, payload_size, severity from base_view, udphdr where base_view.eid=udphdr.eid)
union 
(select eid, time, sig_msg, src_ip, null as src_port, dst_ip, null as dst_port, sig_protocol , sig_id, true_rate, payload_size, severity from base_view where sig_protocol='icmp')
order by eid desc;

CREATE VIEW alert_view AS
    select eid, sig_action, time, sig_msg
    from event, signature 
    where signature.sig_id=event.sig_id;
