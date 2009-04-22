using Sqlp;

int main ( string[] args) {

	var logbook = new Logbook ("test-logbook.db");

	assert (logbook.role is RoleTable);

	var role = logbook.role.new_record () as Role;
	assert (role is Role);

	role.abbreviation = "CA";
	assert (role.abbreviation == "CA");
	role.save ();
	assert (role.id == 1);
	role = logbook.role.find_by_id(1) as Role;
	assert (role is Role);

	assert (role != null);
	assert (role.abbreviation == "CA");

	var flight = logbook.flight.new_record () as Flight;
  	flight.role = role;
  	flight.aircraft = logbook.aircraft.new_record () as Aircraft;
	flight.aircraft.tail = "N12345";
  	flight.aircraft.model = logbook.model.new_record () as Model;
	assert (flight.aircraft.model != null);
	flight.aircraft.model.abbreviation = "PA28";
  	flight.origin = logbook.airport.new_record () as Airport;
  	flight.destination = logbook.airport.new_record () as Airport;

	assert (flight.is_new () == true);
	assert (flight.aircraft is Aircraft);

 	flight.save ();
//	flight.save ();

	var fid = flight.id;
	assert (flight.is_new () == false);

 	flight = logbook.flight.find_by_id (fid) as Flight;
 	assert (flight.is_new () == false);
	assert (flight.role != null);
	assert (flight.role.abbreviation == "CA");
	assert (flight.aircraft != null);
	assert (flight.aircraft.tail == "N12345");
	assert (flight.aircraft.model != null);
	assert (flight.aircraft.model.abbreviation == "PA28");

 	var minneapolis = logbook.airport.new_record () as Airport;
 	minneapolis.icao = "KMSP";
 	minneapolis.iata = "MSP";

 	var thief_river = logbook.airport.new_record () as Airport;
 	thief_river.icao = "KTVF";
 	thief_river.iata = "TVF";

 	var grand_forks = logbook.airport.new_record () as Airport;
 	grand_forks.icao = "KGFK";
 	grand_forks.iata = "GFK";

 	var fargo = logbook.airport.new_record () as Airport;
 	fargo.icao = "KFAR";
 	fargo.iata = "FAR";

	assert (flight.route.length == 0);

	flight.origin = minneapolis;
	flight.destination = grand_forks;

	flight.route.append_airport (fargo);
	flight.route.append_airport (thief_river);

 	flight.save ();
	flight = logbook.flight.find_by_id (fid) as Flight;

	assert (flight.origin.icao == "KMSP");
	assert (flight.origin.iata == "MSP");
	assert (flight.destination.icao == "KGFK");
	assert (flight.destination.iata == "GFK");
	assert (flight.route.length == 2);

	return 0;
}