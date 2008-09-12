BEGIN TRANSACTION;

CREATE TABLE Airports_backup (
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

INSERT INTO Airports_backup SELECT * FROM Airports;
DROP TABLE Airports;

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

INSERT INTO Airports SELECT * FROM Airports_backup;

DROP TABLE Airports_backup;

create unique index airports_icao on airports(icao);
create unique index airports_ident on airports(iata);

COMMIT;