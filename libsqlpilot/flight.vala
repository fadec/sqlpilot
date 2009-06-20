using Sqlite;

namespace Sqlp {
	public class Flight : Record {

		public int64 aircraft_id;
		private Aircraft? _aircraft = null;
		public Aircraft? aircraft {
			get {
				if (_aircraft == null && aircraft_id != 0) {
					_aircraft = (table.database as Logbook).aircraft.find_by_id (aircraft_id) as Aircraft?;
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
					_role = (table.database as Logbook).role.find_by_id (role_id) as Role?;
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
					_origin = (table.database as Logbook).airport.find_by_id (origin_id) as Airport?;
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
					_destination = (table.database as Logbook).airport.find_by_id (destination_id) as Airport?;
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
					_route = new Route ((table.database as Logbook).routing);
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

		private List <Glide> _glides;
		public List <Glide> glides {
			get {
				if (_glides == null) {
					_glides = (table.database as Logbook).glides.find_by_flight_id (id);
				}
				return _glides;
			}
		}

		public Date date;
		public Ordinal leg = Ordinal.invalid ();
		public TimeOfDay actual_out = TimeOfDay ();
		public TimeOfDay actual_in = TimeOfDay ();
		public Duration duration = Duration ();
		public Duration night = Duration ();
		public Duration instrument = Duration ();
		public Duration hood = Duration ();
		public bool cross_country;
		public string notes = "";
		public string flight_number = "";
		public TimeOfDay scheduled_out = TimeOfDay ();
		public TimeOfDay scheduled_in = TimeOfDay ();
		public Duration scheduled_duration = Duration ();
		public string trip = "";
		public Date trip_date;

		public override int bind_for_save (Statement stmt) {
			var i = 1;
			stmt.bind_nonzero_int64				(i++, aircraft_id);
			stmt.bind_nonzero_int64				(i++, role_id);
			stmt.bind_nonzero_int64				(i++, origin_id);
			stmt.bind_nonzero_int64				(i++, destination_id);
			stmt.bind_nonempty_text		(i++, date.to_iso8601 ());

			leg.bind_to_stmt (stmt, i++);

			bind_time_of_day (stmt, i++, actual_out, (origin != null) ? origin.timezone : Timezone ("UTC"));
			bind_time_of_day (stmt, i++, actual_out, Timezone ("UTC"));
			bind_time_of_day (stmt, i++, actual_in, (destination != null) ? destination.timezone : Timezone ("UTC"));
			bind_time_of_day (stmt, i++, actual_in, Timezone ("UTC"));

			duration.bind_to_stmt (stmt, i++); //bind_duration (stmt, i++, duration);
			night.bind_to_stmt (stmt, i++); //bind_duration (stmt, i++, night);
			instrument.bind_to_stmt (stmt, i++); //bind_duration (stmt, i++, instrument);
			hood.bind_to_stmt (stmt, i++); //bind_duration (stmt, i++, hood);

			stmt.bind_int (i++, (int) cross_country);
			stmt.bind_nonempty_text		(i++, notes);
			stmt.bind_nonempty_text		(i++, flight_number);

			bind_time_of_day (stmt, i++, scheduled_out, (origin != null) ? origin.timezone : Timezone ("UTC"));
			bind_time_of_day (stmt, i++, scheduled_out, Timezone ("UTC"));
			bind_time_of_day (stmt, i++, scheduled_in, (destination != null) ? destination.timezone : Timezone ("UTC"));
			bind_time_of_day (stmt, i++, scheduled_in, Timezone ("UTC"));

			scheduled_duration.bind_to_stmt (stmt, i++); //bind_duration (stmt, i++, scheduled_duration);

			stmt.bind_nonempty_text		(i++, trip);
			stmt.bind_nonempty_text		(i++, trip_date.to_iso8601() );
			return i;
		}

		// route member only includes enroute stuff.
		// read_full_route modifies origin and destination.
		public void read_full_route (string str) {
			route.clear ();
			string[] idents = str.split (" ");
			if (idents.length > 0) {
				this.origin = (table.database as Logbook).airport.find_or_create_by_ident (idents[0]) as Airport;
				for (var i=1; i < idents.length - 1; i++) {
					route.append_maybe_airport ((table.database as Logbook).airport.find_or_create_by_ident (idents[i]));
				}
				this.destination = (table.database as Logbook).airport.find_or_create_by_ident (idents[idents.length - 1]);
			} else {
				origin = null;
				destination = null;
			}
		}

		public string show_full_route (Airport.KeyPreference kp) {
			if (origin == null && destination == null && route.length == 0)
				return "";

			var sb = new StringBuilder.sized (64);
			if (origin == null)
				sb.append ("?");
			else
				sb.append (origin.show (kp));

			route.append_to_string_builder (sb, kp);

			sb.append (" ");
			if (destination == null)
				sb.append ("?");
			else
				sb.append (destination.show (kp));

			return sb.str;
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

		public override void set_from_stmt (Statement stmt) {
			var i = 1;
			aircraft_id			= stmt.column_int64 (i++);
			role_id				= stmt.column_int64 (i++);
			origin_id			= stmt.column_int64 (i++);
			destination_id  	= stmt.column_int64 (i++);
			// null roundtrip for date = read null -> "" -> invalid date -> dwrite null
			date				= Date.from_iso8601 (empty_if_null(stmt.column_text (i++)));

			leg					= Ordinal.from_stmt (stmt, i++);

			i++; // skip local ss read
			actual_out			= TimeOfDay.from_stmt (stmt, i++);
			i++; // skip local
			actual_in			= TimeOfDay.from_stmt (stmt, i++);

			duration			= Duration.from_stmt (stmt, i++);
			night				= Duration.from_stmt (stmt, i++);
			instrument			= Duration.from_stmt (stmt, i++);
			hood				= Duration.from_stmt (stmt, i++);
			cross_country		= (bool) stmt.column_int (i++);
			notes				= stmt.column_text (i++);
			flight_number       = stmt.column_text (i++);

			i++; // skip local
			scheduled_out		= TimeOfDay.from_stmt (stmt, i++);
			i++; // skip local
			scheduled_in		= TimeOfDay.from_stmt (stmt, i++);

			scheduled_duration  = Duration.from_stmt (stmt, i++);
			trip				= stmt.column_text (i++);
			trip_date			= Date.from_iso8601 (empty_if_null(stmt.column_text (i++)));
		}

		protected override void before_save () {
			adjust_leg ();
		}

		private void adjust_leg () {
			var flight_table = table as FlightTable;
			if (leg.valid ()) {
				var old_self = flight_table.find_by_id (id) as Flight;
				var leg_conflict = flight_table.find_by_date_leg (date, leg.get ());
				if (leg_conflict != null && leg_conflict.id != this.id) {
					if (old_self != null) {
						leg_conflict.leg = old_self.leg;
						// nuke old_self leg for unique constraint with leg_conflict
						old_self.leg = Ordinal.invalid ();
						old_self.save ();
					} else {
						leg_conflict.leg.set (leg_conflict.leg.get () + 1);
					}
					leg_conflict.save ();
				}
			} else {
				leg.set (flight_table.next_leg (date));
			}
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