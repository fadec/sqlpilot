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
	,origin_airport_id INTEGER
	,destination_airport_id INTEGER
	,Date DATE
	,Leg INTEGER
	,ActualOut TIME
	,ActualOutZ TIME
	,ActualIn TIME
	,ActualInZ TIME
	,Duration INTEGER
	,Night INTEGER
	,Instrument INTEGER
	,Hood INTEGER
	,CrossCountry BOOLEAN
	,Notes TEXT
	-- Schedule stuff: tied to Flights for preflight import simplicity on crew/aircraft/airports
	,FlightNumber VARCHAR
	,ScheduledOut TIME
	,ScheduledOutZ TIME
	,ScheduledIn TIME
	,ScheduledInZ TIME
	,ScheduledDuration INTEGER
	,Trip VARCHAR
	,TripDate DATE
);

CREATE TABLE FlightTags (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,Name CHAR
	,Abbreviation CHAR
);

CREATE TABLE FlightTaggings (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,flight_id INTEGER
	,flight_tag_id INTEGER
);

CREATE TABLE Routing (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,flight_id INTEGER
	,airport_id INTEGER
	,Sequence INTEGER
);

CREATE TABLE Roles (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,Abbreviation CHAR
	,Name CHAR
	,PIC BOOLEAN
	,SIC BOOLEAN
	,FlightEngineer BOOLEAN
	,Solo BOOLEAN
	,DualReceived BOOLEAN
	,Instructor BOOLEAN
	,Military BOOLEAN
	,Total BOOLEAN
);

CREATE TABLE RoleTags (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,Name CHAR
	,Abbreviation CHAR
);

CREATE TABLE RoleTaggings (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,role_id INTEGER
	,role_tag_id INTEGER
);

CREATE TABLE Aircraft (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,model_id INTEGER
	,Registration CHAR
	,Tail CHAR
	,Notes TEXT
);

CREATE TABLE Models (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,Abbreviation CHAR
	,Make CHAR
	,Type CHAR
	,Category CHAR
	,Class CHAR
	,Airplane BOOLEAN
	,Rotorcraft BOOLEAN
	,Glider BOOLEAN
	,Sailplane BOOLEAN
	,LighterThanAir BOOLEAN
	,PoweredLift BOOLEAN
	,Parachute BOOLEAN
	,Paraglider BOOLEAN
	,Powered BOOLEAN
	,Weightshift BOOLEAN
	,Helicopter BOOLEAN
	,Gyroplane BOOLEAN
	,Airship BOOLEAN
	,FreeBalloon BOOLEAN
	,SingleEngine BOOLEAN
	,MultiEngine BOOLEAN
	,FuelInjected BOOLEAN
	,Turbocharged BOOLEAN
	,Radial BOOLEAN
	,Land BOOLEAN
	,Floats BOOLEAN
	,FlyingBoat BOOLEAN
	,Amphibious BOOLEAN
	,Skids BOOLEAN
	,Skis BOOLEAN
	,Turbine BOOLEAN
	,Turbojet BOOLEAN
	,HighPerformance BOOLEAN
	,Retractable BOOLEAN
	,Tailwheel BOOLEAN
	,Complex BOOLEAN
	,Pressurized BOOLEAN
	,Large BOOLEAN
	,Warbird BOOLEAN
	,Experimental BOOLEAN
	,Sport BOOLEAN
	,Ultralight BOOLEAN
	,Footlaunch BOOLEAN
	,Aerobatic BOOLEAN
	,Metal BOOLEAN
	,Composite BOOLEAN
	,Wood BOOLEAN
	,Canard BOOLEAN
	,Biplane BOOLEAN
	,EFIS BOOLEAN
	,Simulator BOOLEAN
	,FlightTrainingDevice BOOLEAN
	,Total BOOLEAN
);

CREATE TABLE ModelTags (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,Name CHAR
	,Abbreviation CHAR
);

CREATE TABLE ModelTaggings (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,model_id INTEGER
	,model_tag_id INTEGER
);

CREATE TABLE Airports (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,ICAO CHAR
	,IATA CHAR
	,Abbreviation CHAR -- For glider launch sites/ships
	,Name CHAR
	,City CHAR
	,Province CHAR
	,Country CHAR
	,Latitude FLOAT
	,Longitude FLOAT
	,Elevation INTEGER
	,OffsetUTC FLOAT
	,OffsetDST FLOAT
	,Timezone CHAR
	,Notes TEXT
);

CREATE TABLE ApproachLandings (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,flight_id INTEGER
	,airport_id INTEGER
	-- Approach Info
	,approach_type_id CHAR
	,Sequence INTEGER
	,ApproachRunway CHAR -- 35L
	,Visibility FLOAT
	,Ceiling FLOAT
	,Coupled BOOLEAN
	-- Landing Info
	,termination_type_id INTEGER
	,surface_id INTEGER
	,LandingRunway CHAR
	,Crosswind FLOAT
	,Night BOOLEAN
	,NightVisionGoggles BOOLEAN
	,Autoland BOOLEAN
);

CREATE TABLE TerminationTypes (
       id INTEGER PRIMARY KEY AUTOINCREMENT
       ,Name CHAR
       ,Landing BOOLEAN
);

CREATE TABLE ApproachTypes (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,Abbreviation CHAR
	,Name CHAR
	,InstrumentCurrency
	,Display
);

CREATE TABLE Holds (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,flight_id INTEGER
	,Location CHAR
	,Turns INTEGER
	,Duration INTEGER
);

CREATE TABLE Takeoffs (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,flight_id INTEGER
	,airport_id INTEGER
	,surface_id INTEGER
	,Sequence INTEGER
	,Runway CHAR -- 35L
	,Visibility FLOAT
	,Crosswind FLOAT
	,Aborted BOOLEAN
	,Night BOOLEAN
);

CREATE TABLE Surfaces (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,Abbreviation CHAR
	,Name CHAR -- grass, pavement-good, pavement-fair, ship, glacier, ...
);

CREATE TABLE Glides (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,flight_id INTEGER
	,launch_type_id INTEGER
	,Sequence INTEGER
	,Duration INTEGER
	,Distance FLOAT
	,ReleaseAltitude FLOAT
	,EngineStartAltitude FLOAT
	,MaxAltitude FLOAT
);

CREATE TABLE LaunchTypes (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,Name CHAR
);

-- 	,Aerotow BOOLEAN
-- 	,Autotow BOOLEAN
-- 	,Winch BOOLEAN
-- 	,SelfLaunch BOOLEAN
-- 	,Hill BOOLEAN
-- 	,Bungee BOOLEAN
-- 	,BalloonDrop BOOLEAN


CREATE TABLE People (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,LastName CHAR
	,FirstName CHAR
	,Badge CHAR
	,Notes CHAR
);

CREATE TABLE Crew (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,flight_id INTEGER
	,person_id INTEGER
	,role_id INTEGER
);

CREATE TABLE Employers (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,Abbreviation CHAR
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
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "FleetNo",  4);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "Model",    5);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "Role",     6);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "Dep",      7);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "Arr",      8);
INSERT INTO Registry (path, key, value) VALUES ("flights/columns/order", "Route",    8);
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
INSERT INTO Registry (path, key, value) VALUES ("flights/view", "RtIATA",   1);
INSERT INTO Registry (path, key, value) VALUES ("flights/view", "RtICAO",   1);
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

create unique	 index airports_icao			 on airports(icao);
create unique	 index airports_iata			 on airports(iata);
create unique	 index airports_abbreviaton		 on airports(abbreviation);
create unique	 index aircraft_registration		 on aircraft(registration);
create unique	 index aircraft_tail			 on aircraft(tail);
create unique	 index roles_abbreviation		 on roles(abbreviation);
create unique	 index model_abbreviation		 on models(abbreviation);

create		 index flights_role_id			 on flights (role_id);
create		 index flights_origin_airport_id	 on flights (origin_airport_id);
create		 index flights_destination_airport_id	 on flights (destination_airport_id);
create		 index flights_aircraft_id		 on flights(aircraft_id);
create		 index flights_date			 on flights(date);
create		 index aircraft_model_id		 on aircraft(model_id);
create		 index routing_flight_id		 on routing(flight_id);
create		 index routing_airport_id		 on routing(airport_id);


-- select flights.id as '_\\id'
--  , a.id as '_\\aircraft_id'
--  , r.id as '_\\role_id'
--  , dep.id as '_\\dep_id'
--  , arr.id as '_\\arr_id'
--  , flights.Date as Date
--  , flights.Leg as Leg
--  , a.tail as Tail
--  , a.fleetno as FleetNo
--  , m.ident as Model
--  , r.ident as Role
--  , dep.iata as DepIATA
--  , dep.icao as DepICAO
--  , arr.iata as ArrIATA
--  , arr.icao as ArrICAO
--  , dep.iata || ' ' || group_concat(rta.iata, ' ') || ' ' || arr.iata AS RtIATA
--  , dep.icao || ' ' || group_concat(rta.icao, ' ') || ' ' || arr.icao AS RtICAO
--  , flights.aout as AOut
--  , flights.AOutUTC as AOutUTC
--  , flights.ain as AIn
--  , flights.AInUTC as AInUTC
--  , m_to_hhmm(flights.dur) as Dur
--  , m_to_hhmm(flights.night) as Night
--  , m_to_hhmm(flights.inst) as Inst
--  , m_to_hhmm(flights.siminst) as SimInst
--  , bool(flights.hold) as Hold
--  , flights.aprch as Aprch
--  , linecount(flights.aprch) as nApr
--  , bool(flights.xc) as XC
--  , round(dist_nm(dep.lat, dep.lon, arr.lat, arr.lon)) as Dist
--  , flights.dland as DLand
--  , flights.nland as NLand
--  , flights.crew as Crew
--  , linecount(flights.crew) as Crw
--  , flights.notes as Notes
--  , linecount(flights.notes) as Nts
--  , flights.fltno as FltNo
--  , flights.sout as SOut
--  , flights.SOutUTC as SOutUTC
--  , flights.sin as SIn
--  , flights.SInUTC as SInUTC
--  , m_to_hhmm(flights.sdur) as SDur
--  , flights.trip as Trip
--  , flights.TripDate as TripDate
--  , m_to_hhmm(flights.dur - flights.sdur) as Over
--   from flights
--   left join aircraft a on flights.aircraft_id = a.id
--   left join models m on a.model_id = m.id
--   left join roles r on flights.role_id = r.id
--   left join airports dep on flights.dep_id = dep.id
--   left join airports arr on flights.arr_id = arr.id
--   left join routing rt on rt.flight_id = flights.id
--   left join airports rta on rt.airport_id = rta.id
--   group by flights.id	



-- CREATE VIEW Routes AS
-- SELECT f.id as flight_id
-- ,dep.id AS dep_id
-- ,arr.id as arr_id
-- ,dep.iata AS DepIATA
-- ,dep.icao AS DepICAO
-- ,arr.iata AS ArrIATA
-- ,arr.icao AS ArrICAO
-- ,dep.lat AS DepLat
-- ,dep.lon AS DepLon
-- ,arr.lat AS ArrLat
-- ,arr.lon AS ArrLon
-- ,dep.iata || ' ' || group_concat(ts.iata, ' ') || ' ' || arr.iata AS RtIATA
-- ,dep.icao || ' ' || group_concat(ts.icao, ' ') || ' ' || arr.icao AS RtICAO
-- FROM flights f
-- LEFT JOIN airports dep ON f.dep_id = dep.id
-- LEFT JOIN airports arr ON f.arr_id = arr.id
-- LEFT JOIN Routing r ON r.flight_id = f.id
-- LEFT JOIN airports ts ON r.airport_id = ts.id
-- GROUP BY f.id;

-- CREATE VIEW Departures AS
-- SELECT airports.id as _id
-- ,airports.iata as IATA
-- ,airports.icao as ICAO
-- ,airports.name as Name
-- ,airports.city as City
-- ,airports.country as Country
-- ,airports.province as Province
-- ,airports.lat as Lat
-- ,airports.lon as Lon
-- ,airports.elev as Elev
-- ,airports.offutc as OffUTC
-- ,airports.offdst as OffDST
-- ,airports.tzone as TZone
-- ,airports.notes as _Notes
-- from airports
-- inner join flights on flights.dep_id = airports.id;

-- CREATE VIEW Arrivals AS
-- SELECT airports.id as _id
-- ,airports.IATA as IATA
-- ,airports.icao as ICAO
-- ,airports.name as Name
-- ,airports.city as City
-- ,airports.country as Country
-- ,airports.province as Province
-- ,airports.lat as Lat
-- ,airports.lon as Lon
-- ,airports.elev as Elev
-- ,airports.offutc as OffUTC
-- ,airports.offdst as OffDST
-- ,airports.tzone as TZone
-- ,airports.notes as _Notes
-- from airports
-- inner join flights on flights.dep_id = airports.id;

-- CREATE VIEW MyAirports AS
-- SELECT * FROM Departures UNION SELECT * FROM Arrivals;

-- CREATE VIEW MyAircraft AS
-- SELECT aircraft.id as id
-- ,aircraft.tail as Tail
-- ,models.ident as Model
-- ,aircraft.fleetno as FleetNo
-- ,aircraft.notes as Notes
-- FROM aircraft
-- LEFT JOIN models on aircraft.model_id = models.id;

-- CREATE VIEW Experience AS
-- SELECT flights.id as _id
-- ,flights.date as Date
-- ,roles.ident as Role
-- ,models.ident as Model
-- ,models.make as Make
-- ,models.type as Type
-- ,aircraft.Tail as Aircraft
-- ,aircraft.fleetno as FleetNo
-- ,flights.sdur as SDur
-- ,flights.dur as Dur
-- ,flights.sout as SOut
-- ,flights.sin as SIn
-- ,flights.aout as AOut
-- ,flights.ain as AIn
-- ,flights.soututc as SOutUTC
-- ,flights.sinutc as SInUTC
-- ,flights.aoututc as AOutUTC
-- ,flights.ainutc as AInUTC
-- ,dep_airports.IATA as DepIATA
-- ,arr_airports.IATA as ArrIATA
-- ,dep_airports.lat as DepLat
-- ,dep_airports.lon as DepLon
-- ,arr_airports.lat as ArrLat
-- ,arr_airports.lon as ArrLon
-- ,dep_airports.iata || ' ' || group_concat(route_airports.iata, ' ') || ' ' || arr_airports.iata AS RtIATA
-- ,dep_airports.icao || ' ' || group_concat(route_airports.icao, ' ') || ' ' || arr_airports.icao AS RtICAO
-- ,round(dist_nm(dep_airports.lat, dep_airports.lon, arr_airports.lat, arr_airports.lon), 1) as Dist
-- ,dep_airports.country as DepCountry
-- ,dep_airports.city as DepCity
-- ,dep_airports.province as DepProvince
-- ,arr_airports.country as ArrCountry
-- ,arr_airports.city as ArrCity
-- ,arr_airports.province as ArrProvice
-- ,flights.night as Night
-- ,flights.inst as Inst
-- ,flights.siminst as SimInst
-- ,flights.xc * flights.dur as XC
-- ,flights.aprch as Aprch
-- ,flights.dur * roles.pic as PIC
-- ,flights.dur * roles.sic as SIC
-- ,flights.dur * roles.fe as FE
-- ,flights.dur * roles.solo as Solo
-- ,flights.dur * roles.dual as Dual
-- ,flights.dur * roles.instruct as Instruct
-- ,flights.dur * models.airplane as Airplane
-- ,flights.dur * models.rotorcraft as Rotorcraft
-- ,flights.dur * models.glider as Glider
-- ,flights.dur * models.lta as LTA
-- ,flights.dur * models.poweredlift as PoweredLift
-- ,flights.dur * models.ppc as PPC
-- ,flights.dur * models.weightshift as Weightshift
-- ,flights.dur * models.heli as Heli
-- ,flights.dur * models.gyro as Gyro
-- ,flights.dur * models.airship as Airship
-- ,flights.dur * models.balloon as Ballon
-- ,flights.dur * models.single as Single
-- ,flights.dur * models.multi as Multi
-- ,flights.dur * models.land as Land
-- ,flights.dur * models.sea as Sea
-- ,flights.dur * models.turbine as Turbine
-- ,flights.dur * models.jet as Jet
-- ,flights.dur * models.highperf as HighPerf
-- ,flights.dur * models.retract as Retract
-- ,flights.dur * models.complex as Complex
-- ,flights.dur * models.pressurized as Pressurized
-- ,flights.dur * models.large as Large
-- ,flights.dur * models.sport as Sport
-- ,flights.dur * models.ultralight as Ultralight
-- ,flights.dur * models.footlaunch as Footlaunch
-- ,flights.dur * models.sim as Sim
-- ,flights.dur * models.ftd as FTD
-- ,flights.dur * roles.total * models.total as Total
-- ,dland as DLand
-- ,nland as NLand
-- FROM flights
-- LEFT JOIN airports dep_airports ON flights.dep_id = dep_airports.id
-- LEFT JOIN airports arr_airports ON flights.arr_id = arr_airports.id
-- LEFT JOIN roles ON flights.role_id = roles.id
-- LEFT JOIN aircraft ON flights.aircraft_id = aircraft.id
-- LEFT JOIN models ON aircraft.model_id = models.id
-- LEFT JOIN routing ON routing.flight_id = flights.id
-- LEFT JOIN airports route_airports ON routing.airport_id = route_airports.id
-- GROUP BY flights.id				
-- ORDER BY flights.date;
