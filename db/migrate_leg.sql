BEGIN TRANSACTION;

CREATE TEMPORARY TABLE Flights_backup (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,aircraft_id INTEGER
	,role_id INTEGER
	,dep_id INTEGER
	,arr_id INTEGER
	,Date DATE
	,DateUTC DATE
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
);


INSERT INTO Flights_backup SELECT * FROM Flights;
DROP TABLE Flights;

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

INSERT INTO Flights SELECT
	id
	,aircraft_id
	,role_id
	,dep_id
	,arr_id
	,Date 
	,NULL
	,AOut 
	,AOutUTC 
	,AIn 
	,AInUTC 
	,Dur 
	,Night 
	,Inst 
	,SimInst 
	,Hold
	,Aprch 
	,XC
	,DLand 
	,NLand 
	,Crew
	,Notes
	,FltNo 
	,SOut 
	,SOutUTC 
	,SIn 
	,SInUTC 
	,SDur 
	,Trip 
	,NULL
FROM Flights_backup;

DROP TABLE Flights_backup;

CREATE INDEX flights_aircraft_id on flights(aircraft_id);
CREATE INDEX flights_arr_id on flights (arr_id);
CREATE INDEX flights_date on flights(date);
CREATE INDEX flights_dep_id on flights (dep_id);
CREATE INDEX flights_role_id on flights (role_id);

COMMIT;