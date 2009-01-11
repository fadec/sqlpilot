CREATE TABLE Routing (
       id INTEGER PRIMARY KEY AUTOINCREMENT
       ,flight_id INTEGER
       ,airport_id INTEGER
       ,Seq INTEGER
);
create index routing_flight_id on routing(flight_id);
create index routing_airport_id on routing(airport_id);
