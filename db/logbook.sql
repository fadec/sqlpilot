CREATE TABLE Flights (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,aircraft_id INTEGER
	,role_id INTEGER
	,dep_id INTEGER
	,arr_id INTEGER
	,Date DATE
	,AOut TIME
	,AIn TIME
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
	,SIn TIME
	,SDur INTEGER
	,Trip VARCHAR
);

CREATE TABLE Roles (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,Ident CHAR
	,Name CHAR
	,PIC BOOLEAN
	,SIC BOOLEAN
	,Solo BOOLEAN
	,Dual BOOLEAN
	,Instruct BOOLEAN
	,Total BOOLEAN
);

CREATE TABLE Aircraft (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,Ident CHAR
	,type_id INTEGER
	,Notes TEXT
);

CREATE TABLE Types (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,Ident CHAR
	,Make CHAR
	,Model CHAR
	,Airplane BOOLEAN
	,Turbine BOOLEAN
	,Jet BOOLEAN
	,Turboprop BOOLEAN
	,Multi BOOLEAN
	,Single BOOLEAN
	,HighPerf BOOLEAN
	,Retract BOOLEAN
	,Complex BOOLEAN
	,Pressurized BOOLEAN
	,Large BOOLEAN
	,Sim BOOLEAN
	,FTD BOOLEAN
	,Land BOOLEAN
	,Sea BOOLEAN
	,Total BOOLEAN
);

CREATE TABLE Airports (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,Ident CHAR
	,ICAO CHAR
	,Name CHAR
	,OffUTC CHAR
	,UseDST BOOLEAN
);

CREATE TABLE Employers (
       id INTEGER PRIMARY KEY AUTOINCREMENT
       ,Ident CHAR
       ,Name CHAR
);

CREATE VIEW LogView AS
SELECT
flights.id as flight_id,
flights.date as date,
flights.sout as sout,
flights.sin as sin,
flights.sdur as sdur,
flights.aout as aout,
flights.ain as ain,
flights.dur as dur,
aircraft.ident as aircraft,
types.ident as type,
dep_airports.ident as dep,
arr_airports.ident as arr,
flights.night as night,
flights.inst as inst,
flights.siminst as siminst,
flights.xc as xc,
flights.dur * roles.pic as pic,
flights.dur * roles.sic as sic,
flights.dur * roles.solo as solo,
flights.dur * roles.dual as dual,
flights.dur * roles.fi as fi,
flights.dur * types.multi as multi,
flights.dur * types.turbine as turbine,
flights.dur * types.jet as jet,
flights.dur * (types.ftd + types.sim) as ftd_sim,
flights.dur * roles.total * types.total as total,
/*COUNT(DISTINCT day_landings.id) as day_landings,
COUNT(DISTINCT night_landings.id) as night_landings,
COUNT(DISTINCT approaches.id) as approaches,*/
flights.fltno as fltno,
flights.trip as trip,
flights.crew as crew,
flights.notes as notes
FROM flights
LEFT JOIN airports dep_airports ON flights.dep_id = dep_airports.id
LEFT JOIN airports arr_airports ON flights.arr_id = arr_airports.id
LEFT JOIN roles ON flights.role_id = roles.id
LEFT JOIN aircraft ON flights.aircraft_id = aircraft.id
LEFT JOIN types ON aircraft.type_id = types.id
/*LEFT JOIN employers ON flights.employer_id = employers.id*/
ORDER BY flights.date, flights.sout;
