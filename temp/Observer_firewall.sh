#firewall NIC : 
#	CentOS / enp0s3 / 192.168.0.3 - default bridge
#	CentOS / enp0s8 / 192.168.56.100 - server gateway
#	CentOS / enp0s9 / 192.168.119.100 - IDS gateway

#server NIC : CentOS  / enp0s3 / 192.168.56.200
#	DB : CentOS / enp0s3 / 192.168.56.201

#IDS NIC : CentOS / enp0s3 / 192.168.119.200
######################################################
#! /bin/sh
#firewall 정책
systemctl stop firewalld.service
echo "[+] firewall stop"

#iptables filter 정책 DROP 시키기
iptables -P INPUT DROP
iptables -P OUTPUT DROP
iptables -P FORWARD DROP
echo "[+] filters set DROP"

#iptables 모든 테이블 초기화 시키기
iptables -t filter -F
iptables -t filter -X
iptables -t nat -F
iptables -t nat -X
iptables -t mangle -F
iptables -t mangle -X
echo "[+] tables init"

#OUTPUT port들 ACCEPT 하기
iptables -A OUTPUT -p tcp -m multiport --dport 21,80 -j ACCEPT
iptables -A OUTPUT -p tcp -m multiport --sport 21,80 -j ACCEPT
iptables -A OUTPUT -p udp -m multiport --dport 53 -j ACCEPT
echo "[+] port OUT ACCEPT"

#tcp 80포트 forward 허용시키기
iptables -A FORWARD -p tcp --dport 80 -j ACCEPT
echo "[+] tcp 80 port ACCEPT"

#udp forward 허용시키기
iptables -A FORWARD -p udp --dport 53 -j ACCEPT
echo "[+] udp ACCEPT"

#iptables 상태기반 정책 적용시키기
iptables -I FORWARD -m conntrack --ctstate ESTABLISHED,RELATED -j ACCEPT
iptables -I INPUT -m conntrack --ctstate ESTABLISHED,RELATED -j ACCEPT
echo "[+] conntrack(ESTABLISHED, RELATED) FORWARD/INPUT ACCPET"

#server에게 가는 패킷들을 IDS(192.168.119.200)에게 mirroring 시키기
iptables -t mangle -I POSTROUTING -d 192.168.56.200 -j TEE --gateway 192.168.119.200
iptables -t mangle -I PREROUTING -s 192.168.56.200 -j TEE --gateway 192.168.119.200
echo "[+] tcp dport 80 / udp / icmp port mirroring to IDS(192.168.119.200)"

#firewall 에게 tcp 80포트 연결(접속)되면 192.168.56.200:80 으로 연결시키기
iptables -t nat -A PREROUTING -p tcp --dport 80 -i enp0s3 -j DNAT --to 192.168.56.200:80 #DNAT를 써서 목적지도달
echo "[+] tcp 80 port DNAT to Server(192.168.56.200:80)"
