CREATE TABLE flights (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,aircraft_id INTEGER
	,role_id INTEGER
	,departure_airport_id INTEGER
	,arrival_airport_id INTEGER
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

CREATE TABLE role_tags (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,name CHAR
);

CREATE TABLE roles_role_tags (
	role_id INTEGER
	,role_tag_id INTEGER
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

CREATE TABLE types_type_tags (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,name CHAR
);

CREATE TABLE type_tags (
	aircraft_type_id INTEGER
	,aircraft_type_tag_id INTEGER
);

CREATE TABLE airports (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,ident CHAR
	,utc_offset CHAR
	,use_dst BOOLEAN
	,name CHAR
);

