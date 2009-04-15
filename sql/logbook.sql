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

PRAGMA locking_mode = EXCLUSIVE;
PRAGMA synchronous = OFF;

PRAGMA auto_vacuum = 0;
PRAGMA encoding = "UTF-8";

CREATE TABLE Flights (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,aircraft_id INTEGER
		REFERENCES Aircraft (id) ON DELETE RESTRICT
	,role_id INTEGER
		REFERENCES Roles (id) ON DELETE RESTRICT
	,origin_airport_id INTEGER
		REFERENCES Airports (id) ON DELETE RESTRICT
	,destination_airport_id INTEGER
		REFERENCES Airports (id) ON DELETE RESTRICT
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
	,CrossCountry BOOLEAN NOT NULL
		DEFAULT 0
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
	,Description CHAR
);

CREATE TABLE FlightTaggings (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,flight_id INTEGER NOT NULL
		REFERENCES Flights (id) ON DELETE CASCADE
	,flight_tag_id INTEGER NOT NULL
		REFERENCES FlightTags (id) ON DELETE CASCADE
);

CREATE TABLE Routing (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,flight_id INTEGER NOT NULL
		REFERENCES Flights (id) ON DELETE CASCADE
	,airport_id INTEGER NOT NULL
		REFERENCES Airports (id) ON DELETE RESTRICT
	,Sequence INTEGER
);

CREATE TABLE Roles (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,Abbreviation CHAR
	,Description CHAR
);

CREATE TABLE RoleTags (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,Name CHAR
	,Description CHAR
);

CREATE TABLE RoleTaggings (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,role_id INTEGER NOT NULL
		REFERENCES Roles (id) ON DELETE CASCADE
	,role_tag_id INTEGER NOT NULL
		REFERENCES RoleTags (id) ON DELETE CASCADE
);

CREATE TABLE Aircraft (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,model_id INTEGER
		REFERENCES Models (id) ON DELETE RESTRICT
	,Registration CHAR
	,Tail CHAR
	,Notes TEXT
);

CREATE TABLE Models (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,Abbreviation CHAR
	,Make CHAR
	,Name CHAR
);

CREATE TABLE ModelTags (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,Name CHAR
	,Description CHAR
);

CREATE TABLE ModelTaggings (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,model_id INTEGER NOT NULL
		REFERENCES Models (id) ON DELETE CASCADE
	,model_tag_id INTEGER NOT NULL
		REFERENCES ModelTags (id) ON DELETE CASCADE
);

CREATE TABLE Airports (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,ICAO CHAR
	,IATA CHAR
	,Abbreviation CHAR -- For glider launch sites/ships
	,Name CHAR
	,City CHAR
	,State CHAR
	,Country CHAR
	,Latitude FLOAT
	,Longitude FLOAT
	,Elevation INTEGER
	,OffsetUTC FLOAT
	,OffsetDST FLOAT
	,Timezone CHAR
	,Notes TEXT
);

CREATE TABLE Approaches (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,flight_id INTEGER NOT NULL
		REFERENCES Flights (id) ON DELETE CASCADE
	,airport_id INTEGER
		REFERENCES Airports (id) ON DELETE RESTRICT
	-- Approach Info
	,approach_type_id INTEGER
		REFERENCES ApproachTypes (id) ON DELETE RESTRICT
	,Sequence INTEGER
	,ApproachRunway CHAR
	,Visibility FLOAT
	,Ceiling FLOAT
	,Coupled BOOLEAN NOT NULL
		DEFAULT 0
	-- Landing Info
	,surface_id INTEGER
		REFERENCES Surfaces (id) ON DELETE RESTRICT
	,Landing BOOLEAN NOT NULL
		DEFAULT 0
	,FullStop BOOLEAN NOT NULL
		DEFAULT 0
	,LandingRunway CHAR
	,Crosswind FLOAT
	,Night BOOLEAN NOT NULL
		DEFAULT 0
	,NightVisionGoggles BOOLEAN NOT NULL
		DEFAULT 0
	,Autoland BOOLEAN NOT NULL
		DEFAULT 0
	,CHECK (Landing OR NOT FullStop)
);

CREATE TABLE ApproachTypes (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,Abbreviation CHAR
	,Name CHAR
	,InstrumentCurrency BOOLEAN NOT NULL
		DEFAULT 0
);

CREATE TABLE Holds (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,flight_id INTEGER NOT NULL
		REFERENCES Flights (id) ON DELETE CASCADE
	,Location CHAR
	,Turns INTEGER
	,Duration INTEGER
);

CREATE TABLE Takeoffs (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,flight_id INTEGER NOT NULL
		REFERENCES Flights (id) ON DELETE CASCADE
	,airport_id INTEGER
		REFERENCES Airports (id) ON DELETE RESTRICT
	,surface_id INTEGER
		REFERENCES Airports (id) ON DELETE RESTRICT
	,Sequence INTEGER
	,Runway CHAR -- 35L
	,Visibility FLOAT
	,Crosswind FLOAT
	,Aborted BOOLEAN NOT NULL
		DEFAULT 0
	,Night BOOLEAN NOT NULL
		DEFAULT 0
);

CREATE TABLE Surfaces (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,Abbreviation CHAR
	,Name CHAR -- grass, pavement-good, pavement-fair, ship, glacier, ...
);

CREATE TABLE Glides (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,flight_id INTEGER NOT NULL
		REFERENCES Flights (id) ON DELETE CASCADE
	,launch_type_id INTEGER
		REFERENCES LaunchTypes (id) ON DELETE RESTRICT
	,Sequence INTEGER
	,Duration INTEGER
	,Distance FLOAT
	,ReleaseAltitude FLOAT
	,MaxAltitude FLOAT
	,EngineStartAltitude FLOAT
);

CREATE TABLE LaunchTypes (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,Abbreviation CHAR
	,Description CHAR
);

CREATE TABLE People (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,LastName CHAR
	,FirstName CHAR
	,Identification CHAR
	,Notes CHAR
);

CREATE TABLE Crew (
	id INTEGER PRIMARY KEY AUTOINCREMENT
	,flight_id INTEGER NOT NULL
		REFERENCES Flights (id) ON DELETE CASCADE
	,person_id INTEGER
		REFERENCES People (id) ON DELETE RESTRICT
	,role_id INTEGER
		REFERENCES Roles (id) ON DELETE RESTRICT
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

INSERT INTO LaunchTypes (Abbreviation) VALUES ("Aerotow");
INSERT INTO LaunchTypes (Abbreviation) VALUES ("Winch");
INSERT INTO LaunchTypes (Abbreviation) VALUES ("Self Launch");
INSERT INTO LaunchTypes (Abbreviation) VALUES ("Hill");
INSERT INTO LaunchTypes (Abbreviation) VALUES ("Bungee");
INSERT INTO LaunchTypes (Abbreviation) VALUES ("Balloon Drop");


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


CREATE UNIQUE	 INDEX airports_icao			 ON airports(icao);
CREATE UNIQUE	 INDEX airports_iata			 ON airports(iata);
CREATE UNIQUE	 INDEX airports_abbreviaton		 ON airports(abbreviation);
CREATE UNIQUE	 INDEX aircraft_registration		 ON aircraft(registration);
CREATE UNIQUE	 INDEX aircraft_tail			 ON aircraft(tail);
CREATE UNIQUE	 INDEX roles_abbreviation		 ON roles(abbreviation);
CREATE UNIQUE	 INDEX model_abbreviation		 ON models(abbreviation);

CREATE		 INDEX flights_role_id			 ON flights (role_id);
CREATE		 INDEX flights_origin_airport_id	 ON flights (origin_airport_id);
CREATE		 INDEX flights_destination_airport_id	 ON flights (destination_airport_id);
CREATE		 INDEX flights_aircraft_id		 ON flights(aircraft_id);
CREATE		 INDEX flights_date			 ON flights(date);
CREATE		 INDEX aircraft_model_id		 ON aircraft(model_id);
CREATE		 INDEX routing_flight_id		 ON routing(flight_id);
CREATE		 INDEX routing_airport_id		 ON routing(airport_id);

CREATE UNIQUE	 INDEX registry_path_key 		 ON Registry(path, key);

--.genfkey --exec
