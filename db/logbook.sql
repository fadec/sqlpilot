CREATE TABLE Flights (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,aircraft_id INTEGER
	,role_id INTEGER
	,dep_id INTEGER
	,arr_id INTEGER
	,Date DATE
	,Seq INTEGER
	,AOut DATETIME
	,AOutUTC DATETIME
	,AIn DATETIME
	,AInUTC DATETIME
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
	,SOut DATETIME
	,SOutUTC DATETIME
	,SIn DATETIME
	,SInUTC DATETIME
	,SDur INTEGER
	,Trip VARCHAR
	,TripDate VARCHAR
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
	,Ident CHAR
	,type_id INTEGER
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
	,Ident CHAR	-- IATA
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

create unique index airports_ident on airports(ident);
create unique index airports_icao on airports(icao);
create unique index aircraft_ident on aircraft(ident);
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
,airports.ident as Ident
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
,airports.ident as Ident
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
,aircraft.ident as Ident
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
,aircraft.ident as Aircraft
,aircraft.fleetno as FleetNo
,flights.dur as Dur
,dep_airports.ident as Dep
,arr_airports.ident as Arr
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
