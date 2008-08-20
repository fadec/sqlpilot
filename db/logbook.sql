--------------------------------------------------------------------------
-- Copyright (C) 2008  Sam Danielson                                    --
--                                                                      --
-- This file is part of Sqlpilot.                                       --
--                                                                      --
-- Sqlpilot is free software: you can redistribute it and/or modify     --
-- it under the terms of the GNU General Public License as published by --
-- the Free Software Foundation, either version 3 of the License, or    --
-- (at your option) any later version.                                  --
--                                                                      --
-- Sqlpilot is distributed in the hope that it will be useful,          --
-- but WITHOUT ANY WARRANTY; without even the implied warranty of       --
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        --
-- GNU General Public License for more details.                         --
--                                                                      --
-- You should have received a copy of the GNU General Public License    --
-- along with Sqlpilot.  If not, see <http://www.gnu.org/licenses/>.    --
--------------------------------------------------------------------------

CREATE TABLE Flights (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,aircraft_id INTEGER
	,role_id INTEGER
	,dep_id INTEGER
	,arr_id INTEGER
	,Date DATE
	,Leg INTEGER
	,AOut TIME
	,AOutUTC TIME
	,AIn TIME
	,AInUTC TIME
	,Dur INTEGER
	,Night INTEGER
	,Inst INTEGER
	,SimInst INTEGER
	,Hold BOOLEAN
	,Aprch VARCHAR
	,XC BOOLEAN
	,DLand INTEGER
	,NLand INTEGER
	,Crew TEXT
	,Notes TEXT
	/* Schedule Stuff */
	,FltNo VARCHAR
	,SOut TIME
	,SOutUTC TIME
	,SIn TIME
	,SInUTC TIME
	,SDur INTEGER
	,Trip VARCHAR
	,TripDate DATE
);

CREATE TABLE Routing (
       id INTEGER PRIMARY KEY AUTOINCREMENT
       ,flight_id INTEGER
       ,airport_id INTEGER
       ,Seq INTEGER
);

CREATE TABLE Roles (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,Ident CHAR
	,Name CHAR
	,PIC BOOLEAN
	,SIC BOOLEAN
	,FE BOOLEAN
	,Solo BOOLEAN
	,Dual BOOLEAN
	,Instruct BOOLEAN
	,Total BOOLEAN
);

CREATE TABLE Aircraft (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,type_id INTEGER
	,Tail CHAR
	,FleetNo CHAR
	,Notes TEXT
);

CREATE TABLE Types (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,Ident CHAR
	,Make CHAR
	,Model CHAR
	,Airplane BOOLEAN
	,Rotorcraft BOOLEAN
	,Glider BOOLEAN
	,LTA BOOLEAN
	,PoweredLift BOOLEAN
	,PPC BOOLEAN
	,Weightshift BOOLEAN
	,Heli BOOLEAN
	,Gyro BOOLEAN
	,Airship BOOLEAN
	,Balloon BOOLEAN
	,Single BOOLEAN
	,Multi BOOLEAN
	,Land BOOLEAN
	,Sea BOOLEAN
	,Turbine BOOLEAN
	,Jet BOOLEAN
	,HighPerf BOOLEAN
	,Retract BOOLEAN
	,Complex BOOLEAN
	,Pressurized BOOLEAN
	,Large BOOLEAN
	,Sport BOOLEAN
	,Ultralight BOOLEAN
	,Footlaunch BOOLEAN
	,Sim BOOLEAN
	,FTD BOOLEAN
	,Total BOOLEAN
);

CREATE TABLE Airports (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,ICAO CHAR
	,IATA CHAR
	,Name CHAR
	,City CHAR
	,Province CHAR
	,Country CHAR
	,Lat FLOAT
	,Lon FLOAT
	,Elev INTEGER
	,OffUTC FLOAT
	,OffDST FLOAT
	,TZone CHAR
	,Notes TEXT
);

CREATE TABLE Employers (
       id INTEGER PRIMARY KEY AUTOINCREMENT
       ,Ident CHAR
       ,Name CHAR
);

CREATE TABLE Registry (
       path CHAR
       ,key INTEGER		-- integer affinity for sort
       ,value INTEGER
);
create unique index registry_path_key on Registry(path, key);

INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "Date",     1);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "FltNo",    2);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "Leg",      3);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "Tail",     4);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "FleetNo",     4);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "Type",     5);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "Role",     6);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "Dep",      7);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "Arr",      8);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "AOut",     9);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "AOutUTC",  10);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "AIn",      11);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "AInUTC",   12);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "Dur",      13);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "Night",    14);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "Inst",     15);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "SimInst",  16);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "Hold",     17);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "Aprch",    18);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "nApr",     19);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "XC",       20);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "Dist",     21);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "DLand",    22);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "NLand",    23);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "Crew",     24);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "Crw",      25);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "Notes",    26);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "Nts",      27);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "SOut",     28);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "SOutUTC",  29);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "SIn",      30);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "SInUTC",   31);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "SDur",     32);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "Trip",     33);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "TripDate", 34);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "Over",     35);

INSERT INTO Registry (path, key, value) VALUES ("flights/view", "Date",     1);
INSERT INTO Registry (path, key, value) VALUES ("flights/view", "FltNo",    1);
INSERT INTO Registry (path, key, value) VALUES ("flights/view", "Leg",      1);
INSERT INTO Registry (path, key, value) VALUES ("flights/view", "Tail",     1);
INSERT INTO Registry (path, key, value) VALUES ("flights/view", "FleetNo",  1);
INSERT INTO Registry (path, key, value) VALUES ("flights/view", "Type",     1);
INSERT INTO Registry (path, key, value) VALUES ("flights/view", "Role",     1);
INSERT INTO Registry (path, key, value) VALUES ("flights/view", "DepIATA",  1);
INSERT INTO Registry (path, key, value) VALUES ("flights/view", "DepICAO",  1);
INSERT INTO Registry (path, key, value) VALUES ("flights/view", "ArrIATA",  1);
INSERT INTO Registry (path, key, value) VALUES ("flights/view", "ArrICAO",  1);
INSERT INTO Registry (path, key, value) VALUES ("flights/view", "AOut",     1);
INSERT INTO Registry (path, key, value) VALUES ("flights/view", "AIn",      1);
INSERT INTO Registry (path, key, value) VALUES ("flights/view", "Dur",      1);
INSERT INTO Registry (path, key, value) VALUES ("flights/view", "Night",    1);
INSERT INTO Registry (path, key, value) VALUES ("flights/view", "Inst",     1);
INSERT INTO Registry (path, key, value) VALUES ("flights/view", "SimInst",  0);
INSERT INTO Registry (path, key, value) VALUES ("flights/view", "Hold",     0);
INSERT INTO Registry (path, key, value) VALUES ("flights/view", "Aprch",    0);
INSERT INTO Registry (path, key, value) VALUES ("flights/view", "nApr",     0);
INSERT INTO Registry (path, key, value) VALUES ("flights/view", "XC",       0);
INSERT INTO Registry (path, key, value) VALUES ("flights/view", "Dist",     0);
INSERT INTO Registry (path, key, value) VALUES ("flights/view", "DLand",    1);
INSERT INTO Registry (path, key, value) VALUES ("flights/view", "NLand",    1);
INSERT INTO Registry (path, key, value) VALUES ("flights/view", "Crew",     0);
INSERT INTO Registry (path, key, value) VALUES ("flights/view", "Crw",      0);
INSERT INTO Registry (path, key, value) VALUES ("flights/view", "Notes",    0);
INSERT INTO Registry (path, key, value) VALUES ("flights/view", "Nts",      0);
INSERT INTO Registry (path, key, value) VALUES ("flights/view", "SOut",     1);
INSERT INTO Registry (path, key, value) VALUES ("flights/view", "SIn",      1);
INSERT INTO Registry (path, key, value) VALUES ("flights/view", "SDur",     1);
INSERT INTO Registry (path, key, value) VALUES ("flights/view", "Trip",     0);
INSERT INTO Registry (path, key, value) VALUES ("flights/view", "TripDate", 0);
INSERT INTO Registry (path, key, value) VALUES ("flights/view", "Over",     0);

INSERT INTO Registry (path, key, value) VALUES ("flights", "UTC", 0);

create unique index airports_icao on airports(icao);
create unique index airports_ident on airports(iata);
create unique index aircraft_tail on aircraft(tail);
create unique index aircraft_fleetno on aircraft(fleetno);
create unique index roles_ident on roles(ident);
create unique index type_ident on types(ident);

create index flights_role_id on flights (role_id);
create index flights_dep_id on flights (dep_id);
create index flights_arr_id on flights (arr_id);
create index flights_aircraft_id on flights(aircraft_id);
create index flights_date on flights(date);
create index aircraft_type_id on aircraft(type_id);

CREATE VIEW Departures AS
SELECT airports.id as _id
,airports.iata as IATA
,airports.icao as ICAO
,airports.name as Name
,airports.city as City
,airports.country as Country
,airports.province as Province
,airports.lat as Lat
,airports.lon as Lon
,airports.elev as Elev
,airports.offutc as OffUTC
,airports.offdst as OffDST
,airports.tzone as TZone
,airports.notes as _Notes
from airports
inner join flights on flights.dep_id = airports.id;

CREATE VIEW Arrivals AS
SELECT airports.id as _id
,airports.IATA as IATA
,airports.icao as ICAO
,airports.name as Name
,airports.city as City
,airports.country as Country
,airports.province as Province
,airports.lat as Lat
,airports.lon as Lon
,airports.elev as Elev
,airports.offutc as OffUTC
,airports.offdst as OffDST
,airports.tzone as TZone
,airports.notes as _Notes
from airports
inner join flights on flights.dep_id = airports.id;

CREATE VIEW MyAirports AS
SELECT * FROM Departures UNION SELECT * FROM Arrivals;

CREATE VIEW MyAircraft AS
SELECT aircraft.id as id
,aircraft.tail as Tail
,types.ident as Type
,aircraft.fleetno as FleetNo
,aircraft.notes as Notes
FROM aircraft
LEFT JOIN types on aircraft.type_id = types.id;

CREATE VIEW Experience AS
SELECT flights.id as _id
,flights.date as Date
,roles.ident as Role
,types.ident as Type
,aircraft.Tail as Aircraft
,aircraft.fleetno as FleetNo
,flights.dur as Dur
,dep_airports.IATA as DepIATA
,arr_airports.IATA as ArrIATA
,dep_airports.lat as DepLat
,dep_airports.lon as DepLon
,arr_airports.lat as ArrLat
,arr_airports.lon as ArrLon
,round(dist_nm(dep_airports.lat, dep_airports.lon, arr_airports.lat, arr_airports.lon), 1) as Dist
,dep_airports.country as DepCountry
,dep_airports.city as DepCity
,dep_airports.province as DepProvince
,arr_airports.country as ArrCountry
,arr_airports.city as ArrCity
,arr_airports.province as ArrProvice
,flights.night as Night
,flights.inst as Inst
,flights.siminst as SimInst
,flights.xc as XC
,flights.dur * roles.pic as PIC
,flights.dur * roles.sic as SIC
,flights.dur * roles.fe as FE
,flights.dur * roles.solo as Solo
,flights.dur * roles.dual as Dual
,flights.dur * roles.instruct as Instruct
,flights.dur * types.airplane as Airplane
,flights.dur * types.rotorcraft as Rotorcraft
,flights.dur * types.glider as Glider
,flights.dur * types.lta as LTA
,flights.dur * types.poweredlift as PoweredLift
,flights.dur * types.ppc as PPC
,flights.dur * types.weightshift as Weightshift
,flights.dur * types.heli as Heli
,flights.dur * types.gyro as Gyro
,flights.dur * types.airship as Airship
,flights.dur * types.balloon as Ballon
,flights.dur * types.single as Single
,flights.dur * types.multi as Multi
,flights.dur * types.land as Land
,flights.dur * types.sea as Sea
,flights.dur * types.turbine as Turbine
,flights.dur * types.jet as Jet
,flights.dur * types.highperf as HighPerf
,flights.dur * types.retract as Retract
,flights.dur * types.complex as Complex
,flights.dur * types.pressurized as Pressurized
,flights.dur * types.large as Large
,flights.dur * types.sport as Sport
,flights.dur * types.ultralight as Ultralight
,flights.dur * types.footlaunch as Footlaunch
,flights.dur * types.sim as Sim
,flights.dur * types.ftd as FTD
,flights.dur * roles.total * types.total as Total
,dland as DLand
,nland as NLand
FROM flights
LEFT JOIN airports dep_airports ON flights.dep_id = dep_airports.id
LEFT JOIN airports arr_airports ON flights.arr_id = arr_airports.id
LEFT JOIN roles ON flights.role_id = roles.id
LEFT JOIN aircraft ON flights.aircraft_id = aircraft.id
LEFT JOIN types ON aircraft.type_id = types.id
ORDER BY flights.date;
