CREATE TABLE flights (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,aircraft_id INTEGER
	,position_id INTEGER
	,scheduled_flight_id INTEGER
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
);

CREATE TABLE scheduled_flights (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,flight_number VARCHAR
	,duty_period_id INTEGER
	,departure_airport_id INTEGER
	,arrival_airport_id INTEGER
	,out_at DATETIME
	,in_at DATETIME
	,duration INTEGER
);

CREATE TABLE duty_periods (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,trip_id INTEGER
	,sequence INTEGER
);

CREATE TABLE trips (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,employer_id INTEGER
	,trip_number VARCHAR
);

CREATE TABLE employers (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,ident CHAR
	,name CHAR
);

CREATE TABLE positions (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,ident CHAR
	,title CHAR
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
	,airplane BOOLEAN
	,glider BOOLEAN
	,ultralight BOOLEAN
	,turbine BOOLEAN
	,turbojet BOOLEAN
	,turboprop BOOLEAN
	,multi_engine BOOLEAN
	,single_engine BOOLEAN
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
);

CREATE TABLE approaches (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,flight_id INTEGER
	,airport_id INTEGER
	,approach_type_id INTEGER
);

CREATE TABLE approach_types (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,ident VARCHAR
);

CREATE TABLE through_stops (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,flight_id INTEGER
	,airport_id INTEGER
	,sequence INTEGER
);


-- pos     emp     ops     trip    crew    notes
-- fltno   sdep    sarr    sout    sin     sdur
-- ident   adep    aarr    aout    ain     adur
-- appch   inst    ldgs    nite    XC

-- 2196    EWR     ALB     19:20   20:05   01+05   XJT
-- ------  ---     ---     -----   -----   *****   EZ962A
-- ---     ---     ---     -----   --      ---     notes

-- CA      XJT     121     EZ952A  crew    notes
-- 2196    EWR     ALB     19:20   20:25   01+05
-- N12345  EWR     ALB     19:18   20:24   01+04
-- ILS     00+45   1n      01+04           more+

--Jepp like view
CREATE VIEW log_view AS
SELECT
flights.id as flight_id
,date(flights.out_at) as departure_date
,aircraft.ident as aircraft
,types.ident as type
,departure_airports.ident as origin
,arrival_airports.ident as destination
,flights.duration as duration
,flights.night as night
,instrument
,simulated_instrument
,flights.duration * cross_country as cross_country
,flights.duration * positions.pic as pic
,flights.duration * positions.sic as sic
,flights.duration * positions.solo as solo
,flights.duration * positions.dual_received as dual_received
,flights.duration * positions.dual_given as dual_given
,flights.duration * types.airplane as airplane
,flights.duration * types.multi_engine as multi_engine
,flights.duration * types.single_engine as single_engine
,flights.duration * types.land as land
,flights.duration * types.sea as sea
,flights.duration * types.turbine as turbine
,flights.duration * types.turbojet as turbojet
,flights.duration * (types.ftd + types.simulator) as ftd_sim
,flights.duration * positions.total * types.total as total
,day_landings
,night_landings
,COUNT(DISTINCT approaches.id) as approaches
,flight_number
,trip_number
,crew
,notes
FROM flights
LEFT JOIN airports departure_airports ON flights.departure_airport_id = departure_airports.id
LEFT JOIN airports arrival_airports ON flights.arrival_airport_id = arrival_airports.id
LEFT JOIN positions ON flights.position_id = positions.id
LEFT JOIN aircraft ON flights.aircraft_id = aircraft.id
LEFT JOIN types ON aircraft.type_id = types.id
LEFT JOIN employers ON flights.employer_id = employers.id
LEFT JOIN approaches ON approaches.flight_id = flights.id
GROUP BY flights.id
ORDER BY flights.out_at;


INSERT INTO approach_types VALUES (1, 'VIS');
INSERT INTO approach_types VALUES (2, 'ILS');
INSERT INTO approach_types VALUES (3, 'GPS');
INSERT INTO approach_types VALUES (4, 'VOR');
INSERT INTO approach_types VALUES (5, 'NDB');

INSERT INTO positions VALUES (1, 'CA', 'Captain', 1, 0, 0, 0, 0, 1);
INSERT INTO positions VALUES (2, 'P', 'Personal', 1, 0, 0, 0, 0, 1);
INSERT INTO positions VALUES (3, 'FO', 'First Officer', 0, 1, 0, 0, 0, 1);

INSERT INTO airports VALUES (1, 'MSP', '-6 hours', 1);
INSERT INTO airports VALUES (2, 'GFK', '-6 hours', 1);
INSERT INTO airports VALUES (3, 'AXN', '-6 hours', 1);

INSERT INTO approaches VALUES (1, 1, 2, 1);
INSERT INTO approaches VALUES (2, 2, 1, 1);

INSERT INTO employers VALUES (1, 'XJT', 'ExpressJet');
INSERT INTO employers VALUES (2, 'PD', 'Piedmont Airlines');

INSERT INTO types VALUES (1, 'EMB145XR', 'Embraer 145 XR', 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1);
INSERT INTO types VALUES (2, 'PA28-161', 'Piper Warrior', 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1);
INSERT INTO types VALUES (3, 'DH8', 'Dash 8', 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1);

INSERT INTO aircraft VALUES (1, 'N12345', 1);
INSERT INTO aircraft VALUES (2, 'N123ND', 2);
INSERT INTO aircraft VALUES (3, 'N1414H', 3);

INSERT INTO flights VALUES (1, 1, 1, 1, 1, 2, '1234', 'Z1287N', '2006-05-25 12:55', '2006-05-25 14:00', 65, '2006-05-25 13:00', '2006-05-25 14:10', 70, 0, 15, 0, 1, 1, 1, 0, '', '');
INSERT INTO flights VALUES (2, 2, 2, 2, 2, 1, NULL, NULL, '2006-05-25 16:30', '2006-05-25 18:55', 145, '2006-05-25 16:30', '2006-05-25 19:00', 150, 0, 30, 0, 1, 0, 1, 0, '', '');
INSERT INTO flights VALUES (3, 3, 2, 3, 1, 3, '4321', 'Q2522B', '2006-01-05 23:30', '2006-01-06 24:14', 44, '2006-01-05 23:30', '2006-01-06 24:15', 45, 44, 0, 0, 0, 1, 0, 1, '', '');
