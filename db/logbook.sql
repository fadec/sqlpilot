CREATE TABLE flights (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,aircraft_ident VARCHAR
	,role_ident VARCHAR
	,departure_airport_ident VARCHAR
	,arrival_airport_ident VARCHAR
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
);

CREATE TABLE roles (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,ident CHAR
	,name CHAR
);

CREATE TABLE role_times (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,ident CHAR
	,role_ident CHAR
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
);

CREATE TABLE type_times (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,ident CHAR
	,type_ident CHAR
);

CREATE TABLE airports (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,ident CHAR
	,utc_offset CHAR
	,use_dst BOOLEAN
	,name CHAR
);

select * from flights f
left join aircraft a on f.aircraft_ident = a.ident
left join types t on t.ident = a.type_ident
left join type_times tt_mel on tt_mel.type_ident = t.ident AND tt_
left join roles r on f.role_ident = r.ident
left join role_times rt_pic on rt_pic.role_ident = rt.ident AND rt_pic.ident = 'PIC'
left join role_times rt_sic on rt_sic.role_ident = r.ident AND rt_sic = 'SIC'
