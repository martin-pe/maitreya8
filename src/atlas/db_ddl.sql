drop table if exists geonames;

create table geonames
(
	featureid integer primary key autoincrement not null,
	name text,
	asciiname text,
	latitude real,
	longitude real,
	feature_class text,
	feature_code text,
	country_code text,
	admin1_code text,
	population int default null,
	elevation int default null,
	timezone text,
	usercode int default 0
);
drop index if exists geonames_name;
create index geonames_name on geonames( name );
drop index if exists geonames_asciiname;
create index geonames_asciiname on geonames( asciiname );

-- Aliases
drop table if exists geonames_aliases;
create table geonames_aliases
(
	featureid int,
	alias text
);

drop index if exists geonames_aliases_featureid;
create index geonames_aliases_featureid on geonames_aliases( featureid );

drop index if exists geonames_aliases_alias;
create index geonames_aliases_alias on geonames_aliases( alias );


-- Timezones
drop table if exists timezones;
create table timezones
(
	name text primary key,
	country_code text,
	offset real
);
drop index if exists timezones_name;
create unique index timezones_name on timezones( name );
create index timezones_country_code on timezones( country_code );

-- Feature Codes
drop table if exists featurecodes;
create table featurecodes
(
	-- char like P
	feature_class text,

	-- char10 like PPLA
	feature_code text,

	description1 text,
	description2 text
);
drop index if exists featurecodes_code;
create unique index featurecodes_pk on featurecodes( feature_class, feature_code );


-- Admin Codes
drop table if exists admincodes;
create table admincodes
(
	country text,
	admin1code text,
	name text,
	asciiname text
);
drop index if exists admincodes_code;
create unique index admincodes_code on admincodes( country, admin1code );

-- Countries
drop table if exists countries;
create table countries
(
	iso text primary key,
	name text not null,
	capital text,
	area int,
	population int,
	continent text not null
);
drop index if exists countries_iso;
create unique index countries_iso on countries( iso );

--
--
--  VIEWS
--
--
drop view if exists atlas;
create view atlas as
SELECT g.featureid as id, g.name as name, g.country_code, g.admin1_code, g.latitude, g.longitude, g.population
FROM geonames g;

drop view if exists atlas_ascii;
create view atlas_ascii as
SELECT g.featureid as id, g.asciiname as name, g.country_code, g.admin1_code, g.latitude, g.longitude, g.population
FROM geonames g;

drop view if exists atlas_alias;
create view atlas_alias as
SELECT a.featureid as id, a.alias as name, g.country_code, g.admin1_code, g.latitude, g.longitude, g.population
FROM geonames_aliases a, geonames g
WHERE a.featureid = g.featureid;


