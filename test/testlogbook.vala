using Sqlp;

int main ( string[] args) {

	var logbook = new Logbook ();
	logbook.open ( "test-logbook.db" );

	assert (logbook.role is RoleCrud);

	var role = logbook.role.new_record ();
	assert (role is Role);

	role.pic = true;
	role.abbreviation = "CA";
	assert (role.pic == true);
	assert (role.abbreviation == "CA");
	role.save ();
	assert (role.id == 1);
	role = logbook.role.find_by_id(1);
	assert (role is Role);

	assert (role != null);
	assert (role.pic == true);
	assert (role.abbreviation == "CA");

	var flight = logbook.flight.new_record ();
  	flight.role = role;
  	flight.aircraft = logbook.aircraft.new_record ();
	flight.aircraft.tail = "N12345";
  	flight.aircraft.model = logbook.model.new_record ();
	assert (flight.aircraft.model != null);
	flight.aircraft.model.abbreviation = "PA28";
  	flight.dep = logbook.airport.new_record ();
  	flight.arr = logbook.airport.new_record ();

	assert (flight.is_new () == true);
	assert (flight.aircraft is Aircraft);

 	flight.save ();
//	flight.save ();

	var fid = flight.id;
	assert (flight.is_new () == false);

 	flight = logbook.flight.find_by_id (fid);
 	assert (flight.is_new () == false);
	assert (flight.role != null);
	assert (flight.role.abbreviation == "CA");
	assert (flight.aircraft != null);
	assert (flight.aircraft.tail == "N12345");
	assert (flight.aircraft.model != null);
	assert (flight.aircraft.model.abbreviation == "PA28");

 	var minneapolis = logbook.airport.new_record ();
 	minneapolis.icao = "KMSP";
 	minneapolis.iata = "MSP";

 	var thief_river = logbook.airport.new_record ();
 	thief_river.icao = "KTVF";
 	thief_river.iata = "TVF";

 	var grand_forks = logbook.airport.new_record ();
 	grand_forks.icao = "KGFK";
 	grand_forks.iata = "GFK";

 	var fargo = logbook.airport.new_record ();
 	fargo.icao = "KFAR";
 	fargo.iata = "FAR";

	assert (flight.route.length == 0);

	flight.dep = minneapolis;
	flight.arr = grand_forks;

	flight.route.append_airport (fargo);
	flight.route.append_airport (thief_river);

 	flight.save ();
	flight = logbook.flight.find_by_id (fid);

	assert (flight.dep.icao == "KMSP");
	assert (flight.dep.iata == "MSP");
	assert (flight.arr.icao == "KGFK");
	assert (flight.arr.iata == "GFK");
	assert (flight.route.length == 2);

	return 0;
}