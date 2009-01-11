BEGIN TRANSACTION;

CREATE TABLE Aircraft_backup (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,Ident CHAR
	,type_id INTEGER
	,FleetNo CHAR
	,Notes TEXT
);


INSERT INTO Aircraft_backup SELECT * FROM Aircraft;
DROP TABLE Aircraft;

CREATE TABLE Aircraft (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,type_id INTEGER
	,Tail CHAR
	,FleetNo CHAR
	,Notes TEXT
);

INSERT INTO Aircraft SELECT
       id
       ,type_id
       ,Ident
       ,FleetNo
       ,Notes
FROM Aircraft_backup;

DROP TABLE Aircraft_backup;

UPDATE Aircraft SET Tail = NULL where Tail = "";
UPDATE Aircraft SET FleetNo = NULL where FleetNo = "";

CREATE UNIQUE INDEX aircraft_fleetno on aircraft(fleetno);
CREATE UNIQUE INDEX aircraft_tail on aircraft(tail);
CREATE INDEX aircraft_type_id on aircraft(type_id);

COMMIT;