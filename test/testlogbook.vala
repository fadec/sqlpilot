using SqlPilot;

int main ( string[] args) {

	var logbook = new Logbook ();
	logbook.open ( "test-logbook.db" );

	var role = logbook.role.beget ();
	assert (role is Role);
	role.pic = true;
	role.ident = "CA";
	assert (role.pic == true);
	assert (role.ident == "CA");
	role.save ();
	role = logbook.role.find_by_id(1);
	assert (role is Role);
	assert (role != null);
	assert (role.pic == true);
	assert (role.ident == "CA");

	var flight = logbook.flight.beget ();
  	flight.role = role;
  	flight.aircraft = logbook.aircraft.beget ();
	flight.aircraft.tail = "N12345";
  	flight.aircraft.model = logbook.model.beget ();
	assert (flight.aircraft.model != null);
	flight.aircraft.model.ident = "PA28";
  	flight.dep = logbook.airport.beget ();
  	flight.arr = logbook.airport.beget ();

	assert (flight.is_new () == true);
	assert (flight.aircraft is Aircraft);

 	flight.save ();
//	flight.save ();

	var fid = flight.id;
	assert (flight.is_new () == false);

 	flight = logbook.flight.find_by_id (fid);
 	assert (flight.is_new () == false);
	assert (flight.role != null);
	assert (flight.role.ident == "CA");
	assert (flight.aircraft != null);
	assert (flight.aircraft.tail == "N12345");
	assert (flight.aircraft.model != null);
	assert (flight.aircraft.model.ident == "PA28");

 	var minneapolis = logbook.airport.beget ();
 	minneapolis.icao = "KMSP";
 	minneapolis.iata = "MSP";

 	var thief_river = logbook.airport.beget ();
 	thief_river.icao = "KTVF";
 	thief_river.iata = "TVF";

 	var grand_forks = logbook.airport.beget ();
 	grand_forks.icao = "KGFK";
 	grand_forks.iata = "GFK";

 	var fargo = logbook.airport.beget ();
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