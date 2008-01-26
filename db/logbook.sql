CREATE TABLE flights (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,aircraft VARCHAR
	,role VARCHAR
	,depart VARCHAR
	,arrive VARCHAR
	,out_at DATETIME
	,in_at DATETIME
	,duration INTEGER
	,night INTEGER
	,instrument INTEGER
	,simulated_instrument INTEGER
	,holding BOOLEAN
	,cross_country BOOLEAN
	,day_landings INTEGER
	,night_landings INTEGER
	,crew TEXT
	,notes TEXT
	/* Schedule Stuff */
	,flight_number VARCHAR
	,scheduled_out_at DATETIME
	,scheduled_in_at DATETIME
	,scheduled_duration INTEGER
	,trip VARCHAR
);

CREATE TABLE roles (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,ident CHAR
	,name CHAR
	,pic BOOLEAN
	,sic BOOLEAN
	,solo BOOLEAN
	,dual_given BOOLEAN
	,dual_received BOOLEAN
	,total BOOLEAN
);

CREATE TABLE aircraft (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,ident CHAR
	,type_id INTEGER
);

CREATE TABLE types (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,ident CHAR
	,name CHAR
	,category VARCHAR
	,class VARCHAR
	,turbine BOOLEAN
	,turbojet BOOLEAN
	,turboprop BOOLEAN
	,multi_engine BOOLEAN
	,single_engline BOOLEAN
	,high_performance BOOLEAN
	,retractable BOOLEAN
	,complex BOOLEAN
	,pressurized BOOLEAN
	,large BOOLEAN
	,simulator BOOLEAN
	,ftd BOOLEAN
	,land BOOLEAN
	,sea BOOLEAN
	,total BOOLEAN
);

CREATE TABLE airports (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,ident CHAR
	,utc_offset CHAR
	,use_dst BOOLEAN
	,name CHAR
);

