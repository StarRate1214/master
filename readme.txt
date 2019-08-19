//mysql 설치
yum -y install http://dev.mysql.com/get/mysql57-community-release-el7-11.noarch.rpm
yum -y install mysql-community-server
systemctl start mysqld
systemctl enable mysqld

//mysql 설정
grep password /var/log/mysqld.log 에서 root 패스워드 가져오기
mysql -uroot -p
비밀번호 입력
mysql> ALTER USER 'root'@'localhost' IDENTIFIED BY 'ZPelHokYi3/b';
mysql> FLUSH PRIVILEGES;
mysql> create user 'jwh'@'localhost' identified by 'Qwer!234';
mysql> grant all privileges on *.* to 'jwh'@'localhost';
mysql> create database test;

//라이브러리 설치
yum install -y libconfig-devel mysql-connector-c++ mysql-connector-c++-devel boost boost-thread boost-devel libpcap-devel  pcre-devel

//php설치
yum install wget -y
wget https://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm
rpm -Uvh epel-release-latest-7.noarch.rpm
wget http://rpms.remirepo.net/enterprise/remi-release-7.rpm
rpm -Uvh remi-release-7.rpm
yum install -y yum-utils
yum-config-manager --enable remi-php72
yum install -y php php-xml php-mysql
vi /etc/httpd/conf/httpd.conf
119 /var/www/html -> /var/www/html/http
164 index.php 추가
306 AddType application/x-httpd-php .html .htm .php .inc
    AddType application/x-httpd-php-source .phps
    추가
systemctl start httpd
systemctl enable httpd



//컴파일
g++ main.cpp Rules/*.cpp DB/*.cpp Capture/*.cpp -I HEADER/ -std=c++11 -I /usr/include/mysql-cppconn-8/mysql/ -L /usr/lib64 -lmysqlcppconn -lconfig++ -lpcrecpp -lpcap -o main -g


//실행
sudo ./main