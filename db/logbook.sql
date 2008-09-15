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
	,model_id INTEGER
	,Tail CHAR
	,FleetNo CHAR
	,Notes TEXT
);

CREATE TABLE Models (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,Ident CHAR
	,Make CHAR
	,Type CHAR
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

CREATE TABLE Reports (
       id INTEGER PRIMARY KEY AUTOINCREMENT
       ,Title CHAR
       ,SQL TEXT
);
create unique index reports_title on Reports(title);

CREATE TABLE Registry (
       path CHAR
       ,key INTEGER		-- integer affinity for sort
       ,value INTEGER
);
create unique index registry_path_key on Registry(path, key);

INSERT INTO Reports (Title, SQL) VALUES ("Time in Model", "select Model, count(*) as Flights, hm(sum(pic)) as PIC, hm(sum(sic)) as SIC, sum(dist) as Distance, hm(sum(dur)) as Total, hm(avg(dur)) as AvgDur, hm(sum(inst)) as Inst, hm(sum(night)) as Night from experience group by model;");

INSERT INTO Reports (Title, SQL) VALUES ("Daily", "select Date, hm(s) as Scheduled, hm(a) as Actual from (select date, sum(sdur) as s, sum(dur) as a from Experience group by date) order by Date;");

INSERT INTO Reports (Title, SQL) VALUES ("Monthly", "select strftime('%Y-%m', Date) as Month, count(id) as Flights, hm(sum(SDur)) as Scheduled, hm(sum(Dur)) as Actual from Flights group by Month order by Month;");


INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "Date",     1);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "FltNo",    2);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "Leg",      3);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "Tail",     4);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "FleetNo",     4);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "Model",     5);
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
INSERT INTO Registry (path, key, value) VALUES ("flights/view", "Model",     1);
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
create unique index model_ident on models(ident);

create index flights_role_id on flights (role_id);
create index flights_dep_id on flights (dep_id);
create index flights_arr_id on flights (arr_id);
create index flights_aircraft_id on flights(aircraft_id);
create index flights_date on flights(date);
create index aircraft_model_id on aircraft(model_id);
create index routing_flight_id on routing(flight_id);
create index routing_airport_id on routing(airport_id);


CREATE VIEW Routes AS
SELECT f.id as flight_id
,dep.id AS dep_id
,arr.id as arr_id
,dep.iata AS DepIATA
,dep.icao AS DepICAO
,arr.iata AS ArrIATA
,arr.icao AS ArrICAO
,dep.lat AS DepLat
,dep.lon AS DepLon
,arr.lat AS ArrLat
,arr.lon AS ArrLon
,dep.iata || ' ' || group_concat(ts.iata, ' ') || ' ' || arr.iata AS RtIATA
,dep.icao || ' ' || group_concat(ts.icao, ' ') || ' ' || arr.icao AS RtICAO
FROM flights f
LEFT JOIN airports dep ON f.dep_id = dep.id
LEFT JOIN airports arr ON f.arr_id = arr.id
LEFT JOIN Routing r ON r.flight_id = f.id
LEFT JOIN airports ts ON r.airport_id = ts.id
GROUP BY f.id;

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
,models.ident as Model
,aircraft.fleetno as FleetNo
,aircraft.notes as Notes
FROM aircraft
LEFT JOIN models on aircraft.model_id = models.id;

CREATE VIEW Experience AS
SELECT flights.id as _id
,flights.date as Date
,roles.ident as Role
,models.ident as Model
,models.make as Make
,models.type as Type
,aircraft.Tail as Aircraft
,aircraft.fleetno as FleetNo
,flights.sdur as SDur
,flights.dur as Dur
,flights.sout as SOut
,flights.sin as SIn
,flights.aout as AOut
,flights.ain as AIn
,flights.soututc as SOutUTC
,flights.sinutc as SInUTC
,flights.aoututc as AOutUTC
,flights.ainutc as AInUTC
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
,flights.xc * flights.dur as XC
,flights.aprch as Aprch
,flights.dur * roles.pic as PIC
,flights.dur * roles.sic as SIC
,flights.dur * roles.fe as FE
,flights.dur * roles.solo as Solo
,flights.dur * roles.dual as Dual
,flights.dur * roles.instruct as Instruct
,flights.dur * models.airplane as Airplane
,flights.dur * models.rotorcraft as Rotorcraft
,flights.dur * models.glider as Glider
,flights.dur * models.lta as LTA
,flights.dur * models.poweredlift as PoweredLift
,flights.dur * models.ppc as PPC
,flights.dur * models.weightshift as Weightshift
,flights.dur * models.heli as Heli
,flights.dur * models.gyro as Gyro
,flights.dur * models.airship as Airship
,flights.dur * models.balloon as Ballon
,flights.dur * models.single as Single
,flights.dur * models.multi as Multi
,flights.dur * models.land as Land
,flights.dur * models.sea as Sea
,flights.dur * models.turbine as Turbine
,flights.dur * models.jet as Jet
,flights.dur * models.highperf as HighPerf
,flights.dur * models.retract as Retract
,flights.dur * models.complex as Complex
,flights.dur * models.pressurized as Pressurized
,flights.dur * models.large as Large
,flights.dur * models.sport as Sport
,flights.dur * models.ultralight as Ultralight
,flights.dur * models.footlaunch as Footlaunch
,flights.dur * models.sim as Sim
,flights.dur * models.ftd as FTD
,flights.dur * roles.total * models.total as Total
,dland as DLand
,nland as NLand
FROM flights
LEFT JOIN airports dep_airports ON flights.dep_id = dep_airports.id
LEFT JOIN airports arr_airports ON flights.arr_id = arr_airports.id
LEFT JOIN roles ON flights.role_id = roles.id
LEFT JOIN aircraft ON flights.aircraft_id = aircraft.id
LEFT JOIN models ON aircraft.model_id = models.id
ORDER BY flights.date;
