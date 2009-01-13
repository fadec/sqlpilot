using SqlPilot;

int main ( string[] args) {

	var logbook = new Logbook ();
	logbook.open ( "test-logbook.db" );

	var role = logbook.role.new_record () as Role;
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
  	flight.aircraft.model = logbook.model.beget ();
  	flight.dep = logbook.airport.beget ();
  	flight.arr = logbook.airport.beget ();

	assert (flight.is_new == true);
	assert (flight.aircraft is Aircraft);

	flight.save ();

	assert (flight.is_new == false);

	return 0;
}