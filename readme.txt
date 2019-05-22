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
yum install -y libconfig-devel mysql-connector-c++ mysql-connector-c++-devel boost boost-thread boost-devel

//컴파일
g++ main.cpp Rules/*.cpp DB/*.cpp Capture/*.cpp -I HEADER/ -std=c++11 -I /usr/include/mysql-cppconn-8/mysql/ -L /usr/lib64 -lmysqlcppconn -lconfig++ -lpcrecpp -o main