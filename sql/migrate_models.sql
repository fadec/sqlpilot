BEGIN TRANSACTION;

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

INSERT INTO Models SELECT * FROM Types;
DROP TABLE Types;

CREATE TABLE Aircraft_backup (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,type_id INTEGER
	,Tail CHAR
	,FleetNo CHAR
	,Notes TEXT
);

INSERT INTO Aircraft_backup SELECT * from Aircraft;
DROP TABLE Aircraft;

CREATE TABLE Aircraft (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,model_id INTEGER
	,Tail CHAR
	,FleetNo CHAR
	,Notes TEXT
);

INSERT INTO Aircraft SELECT * from Aircraft_backup;
DROP TABLE Aircraft_backup;

CREATE UNIQUE INDEX aircraft_fleetno on aircraft(fleetno);
CREATE UNIQUE INDEX aircraft_tail on aircraft(tail);
CREATE INDEX aircraft_type_id on aircraft(model_id);

DROP VIEW Experience;
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

DELETE FROM Reports;
INSERT INTO Reports (Title, SQL) VALUES ("Time in Model", "select Model, count(*) as Flights, hm(sum(pic)) as PIC, hm(sum(sic)) as SIC, sum(dist) as Distance, hm(sum(dur)) as Total, hm(avg(dur)) as AvgDur, hm(sum(inst)) as Inst, hm(sum(night)) as Night from experience group by model;");

INSERT INTO Reports (Title, SQL) VALUES ("Daily", "select Date, hm(s) as Scheduled, hm(a) as Actual from (select date, sum(sdur) as s, sum(dur) as a from Experience group by date) order by Date;");

INSERT INTO Reports (Title, SQL) VALUES ("Monthly", "select strftime('%Y-%m', Date) as Month, count(id) as Flights, hm(sum(SDur)) as Scheduled, hm(sum(Dur)) as Actual from Flights group by Month order by Month;");


COMMIT;