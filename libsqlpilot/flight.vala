using Sqlite;

namespace Sqlp {
	public class Flight : Record <FlightTable> {

		public int64 aircraft_id;
		private Aircraft? _aircraft = null;
		public Aircraft? aircraft {
			get {
				if (_aircraft == null && aircraft_id != 0) {
					_aircraft = table.database.aircraft.find_by_id (aircraft_id);
				}
				return _aircraft;
			}
			set {
				_aircraft = value;
				aircraft_id = (value == null) ? 0 : value.id;
			}
		}

		public int64 role_id;
		private Role? _role;
		public Role? role {
			get {
				if (_role == null && role_id != 0) {
					_role = table.database.role.find_by_id (role_id);
				}
				return _role;
			}
			set {
				_role = value;
				role_id = (value == null) ? 0 : value.id;
			}
		}

		public int64 origin_id;
		private Airport? _origin;
		public Airport? origin {
			get {
				if (_origin == null && origin_id != 0) {
					_origin = table.database.airport.find_by_id (origin_id);
				}
				return _origin;
			}
			set {
				_origin = value;
				if (value == null) {
					origin_id = 0;
				} else {
					origin_id = value.id;
				}
			}
		}


		public int64 destination_id;
		private Airport? _destination;
		public Airport? destination {
			get {
				if (_destination == null && destination_id != 0) {
					_destination = table.database.airport.find_by_id (destination_id);
				}
				return _destination;
			}
			set {
				_destination = value;
				if (value == null) {
					destination_id = 0;
				} else {
					destination_id = value.id;
				}
			}
		}


		private Route? _route;
		public Route route {
			get {
				if (_route == null) {
					_route = new Route (table.database.routing);
					_route.flight = this;
					_route.lookup ();
				}
				return _route;
			}
			set {
				if (value == null) _route.clear;
				else _route = value;
			}
		}

		public Date date;
		public int leg;
		public TimeOfDay actual_out = TimeOfDay ();
		public TimeOfDay actual_in = TimeOfDay ();
		public Duration duration;
		public Duration night;
		public Duration instrument;
		public Duration hood;
		public bool cross_country;
		public string notes = "";
		public string flight_number = "";
		public TimeOfDay scheduled_out = TimeOfDay ();
		public TimeOfDay scheduled_in = TimeOfDay ();
		public Duration scheduled_duration;
		public string trip = "";
		public Date trip_date;

		public override int bind_for_save (Statement stmt) {
			var i = 1;
			stmt.bind_int64				(i++, aircraft_id);
			stmt.bind_int64				(i++, role_id);
			stmt.bind_int64				(i++, origin_id);
			stmt.bind_int64				(i++, destination_id);
			stmt.bind_nonempty_text		(i++, date.to_iso8601 ());
			stmt.bind_int				(i++, leg);

			bind_time_of_day (stmt, i++, actual_out, (origin != null) ? origin.timezone : Timezone ("UTC"));
			bind_time_of_day (stmt, i++, actual_out, Timezone ("UTC"));
			bind_time_of_day (stmt, i++, actual_in, (destination != null) ? destination.timezone : Timezone ("UTC"));
			bind_time_of_day (stmt, i++, actual_in, Timezone ("UTC"));

			bind_duration (stmt, i++, duration);
			bind_duration (stmt, i++, night);
			bind_duration (stmt, i++, instrument);
			bind_duration (stmt, i++, hood);

			stmt.bind_int (i++, (int) cross_country);
			stmt.bind_nonempty_text		(i++, notes);
			stmt.bind_nonempty_text		(i++, flight_number);

			bind_time_of_day (stmt, i++, scheduled_out, (origin != null) ? origin.timezone : Timezone ("UTC"));
			bind_time_of_day (stmt, i++, scheduled_out, Timezone ("UTC"));
			bind_time_of_day (stmt, i++, scheduled_in, (destination != null) ? destination.timezone : Timezone ("UTC"));
			bind_time_of_day (stmt, i++, scheduled_in, Timezone ("UTC"));

			bind_duration (stmt, i++, scheduled_duration);

			stmt.bind_nonempty_text		(i++, trip);
			stmt.bind_nonempty_text		(i++, trip_date.to_iso8601() );
			return i;
		}

		public void read_full_route (string str) {
			string[] idents = str.split (" ");
			if (idents.length > 0) {
				this.origin = table.database.airport.find_or_create_by_ident (idents[0]);
				for (var i=1; i < idents.length - 1; i++) {
					route.append_maybe_airport (table.database.airport.find_or_create_by_ident (idents[i]));
				}
				this.destination = table.database.airport.find_or_create_by_ident (idents[idents.length - 1]);
			}
		}

		public string show_full_route_icao () {
			return (origin != null ? origin.icao : "?") + " " + (destination != null ? destination.icao : "?");
		}

		public string show_full_route_iata () {
			return (origin != null ? origin.iata : "?") + " " + (destination != null ? destination.iata : "?");
		}

		private void bind_time_of_day (Statement stmt, int iter, TimeOfDay tod, Timezone in_timezone) {
			if (tod.valid ()) {
				string str;
				if (date.valid ()) {
					var dt = Datetime (date, tod).in_timezone (in_timezone);
					str = dt.time_of_day.to_iso8601 ();
				} else {
					str = tod.to_iso8601 ();
				}
				stmt.bind_text (iter, str);
			} else {
				stmt.bind_null (iter);
			}
		}

		private void bind_duration (Statement stmt, int iter, Duration dur) {
			if (dur.to_minutes () > 0) {
				stmt.bind_int64 (iter, dur.to_minutes ());
			} else {
				stmt.bind_null (iter);
			}
		}

		public override void set_from_stmt (Statement stmt) {
			var i = 1;
			aircraft_id			= stmt.column_int64 (i++);
			role_id				= stmt.column_int64 (i++);
			origin_id			= stmt.column_int64 (i++);
			destination_id  	= stmt.column_int64 (i++);
			// null roundtrip for date = read null -> "" -> invalid date -> dwrite null
			date				= Date.from_iso8601 (empty_if_null(stmt.column_text (i++)));
			leg					= stmt.column_int   (i++);
			i++; // skip local ss read
			actual_out			= TimeOfDay.from_tzname_time ("UTC", stmt.column_text (i++));
			i++; // skip local
			actual_in			= TimeOfDay.from_tzname_time ("UTC", stmt.column_text (i++));
			duration			= Duration.from_minutes (stmt.column_int (i++));
			night				= Duration.from_minutes (stmt.column_int (i++));
			instrument			= Duration.from_minutes (stmt.column_int (i++));
			hood				= Duration.from_minutes (stmt.column_int (i++));
			cross_country		= (bool) stmt.column_text (i++);
			notes				= stmt.column_text (i++);
			flight_number       = stmt.column_text (i++);
			i++; // skip local
			scheduled_out		= TimeOfDay.from_tzname_time ("UTC", stmt.column_text (i++));
			i++; // skip local
			scheduled_in		= TimeOfDay.from_tzname_time ("UTC", stmt.column_text (i++));
			scheduled_duration  = Duration.from_minutes (stmt.column_int (i++));
			trip				= stmt.column_text (i++);
			trip_date			= Date.from_iso8601 (empty_if_null(stmt.column_text (i++)));
//			stderr.printf("WTF: %s\n", sout.timezone.name);
		}

		protected override bool save_dependencies () {
			if (aircraft != null && aircraft.save ()) aircraft_id = aircraft.id;
			if (role != null && role.save ()) role_id = role.id;
			if (origin != null && origin.save ()) origin_id = origin.id;
			if (destination != null && destination.save ()) destination_id = destination.id;
			return true;
		}

		protected override bool save_dependents () {
			route.save ();
			return true;
		}

		public override string summary () {
			return "uu";
		}
	}
}