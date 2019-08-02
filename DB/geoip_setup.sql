#how to use it
# 1. geocsv.tar.gz 압축을 푼다.  
# 2. 아래 LOAD DATA LOCAL INFILE 의 경로를 알맞게 바꿔준다. 
# 3. 전체 쿼리를 실행한다. 

CREATE DATABASE geoip;
use geoip;

CREATE TABLE `ip_blocks` (

`ip_from` int unsigned NOT NULL,
`ip_to` int unsigned NOT NULL,

  `network` varchar(32) NOT NULL,
  `geoname_id` int unsigned NOT NULL,
  `registered_country_geoname_id` int unsigned NOT NULL,
  `represented_country_geoname_id` int unsigned NOT NULL,
  `is_anonymous_proxy` tinyint(1) NOT NULL,
  `is_satellite_provider` tinyint(1) NOT NULL,
  `postal_code` varchar(32) NOT NULL,
  `latitude` float(8,4) NOT NULL,
  `longitude` float(8,4) NOT NULL,
  `accuracy_radius` smallint unsigned NOT NULL
);

CREATE TABLE `ip_locations` (
  `geoname_id` int unsigned NOT NULL,
  `locale_code` varchar(32) NOT NULL,
  `continent_code` char(2) NOT NULL,
  `continent_name` varchar(32) NOT NULL,
  `country_iso_code` char(2) NOT NULL,
  `country_name` varchar(64) NOT NULL,
  `subdivision_1_iso_code` varchar(3) NOT NULL,
  `subdivision_1_name` varchar(128) COLLATE 'utf8_unicode_ci' NOT NULL,
  `subdivision_2_iso_code` varchar(3) NOT NULL,
  `subdivision_2_name` varchar(128) COLLATE 'utf8_unicode_ci' NOT NULL,
  `city_name` varchar(128) COLLATE 'utf8_unicode_ci' NOT NULL,
  `metro_code` smallint unsigned NOT NULL,
  `time_zone` varchar(64) NOT NULL
);

LOAD DATA LOCAL INFILE '/home/kit/project/master/DB/GeoLite2-City-Blocks-IPv4.csv' INTO TABLE ip_blocks COLUMNS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"' IGNORE 1 LINES (
@network,
geoname_id,
registered_country_geoname_id,
represented_country_geoname_id,
is_anonymous_proxy,
is_satellite_provider,
postal_code,
latitude,
longitude,
accuracy_radius) SET 
ip_from = INET_ATON(SUBSTRING(@network, 1, LOCATE('/', @network) - 1)),
ip_to = (INET_ATON(SUBSTRING(@network, 1, LOCATE('/', @network) - 1)) + (pow(2, (32-CONVERT(SUBSTRING(@network, LOCATE('/', @network) + 1), UNSIGNED INTEGER)))-1));


LOAD DATA LOCAL INFILE '/home/kit/project/master/DB/GeoLite2-City-Locations-en.csv' INTO TABLE ip_locations CHARACTER SET UTF8 COLUMNS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"' IGNORE 1 LINES (
geoname_id,
locale_code,
continent_code,
continent_name,
country_iso_code,
country_name,
subdivision_1_iso_code,
subdivision_1_name,
subdivision_2_iso_code,
subdivision_2_name,
city_name,
metro_code,
time_zone);

ALTER TABLE `ip_blocks` ADD PRIMARY KEY `ip_to` (`ip_to`);
ALTER TABLE `ip_locations` ADD PRIMARY KEY `geoname_id` (`geoname_id`);

DELIMITER $$
CREATE FUNCTION `IP2Location`(`ip` varchar(50))
	RETURNS int(11)
	LANGUAGE SQL
	DETERMINISTIC
	CONTAINS SQL
	SQL SECURITY DEFINER
	COMMENT ''
BEGIN

DECLARE loc_id INT;

SELECT geoname_id INTO loc_id FROM ip_blocks WHERE ip_to >= INET_ATON(TRIM(ip)) and ip_from < INET_ATON(TRIM(ip)) ORDER BY ip_to LIMIT 1;

RETURN IFNULL(loc_id, 0);

END$$