using Sqlite;

namespace Sqlp {
	public class Flight : Record {

		public int64 aircraft_id;
		private Aircraft? _aircraft = null;
		public Aircraft? aircraft {
			get {
				if (_aircraft == null && aircraft_id != 0) {
					_aircraft = crud.logbook.aircraft.find_by_id (aircraft_id);
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
					_role = crud.logbook.role.find_by_id (role_id);
				}
				return _role;
			}
			set {
				_role = value;
				role_id = (value == null) ? 0 : value.id;
			}
		}

		public int64 dep_id;
		private Airport? _dep;
		public Airport? dep {
			get {
				if (_dep == null && dep_id != 0) {
					_dep = crud.logbook.airport.find_by_id (dep_id);
				}
				return _dep;
			}
			set {
				_dep = value;
				if (value == null) {
					dep_id = 0;
				} else {
					dep_id = value.id;
				}
			}
		}


		public int64 arr_id;
		private Airport? _arr;
		public Airport? arr {
			get {
				if (_arr == null && arr_id != 0) {
					_arr = crud.logbook.airport.find_by_id (arr_id);
				}
				return _arr;
			}
			set {
				_arr = value;
				if (value == null) {
					arr_id = 0;
				} else {
					arr_id = value.id;
				}
			}
		}


		private Route? _route;
		public Route route {
			get {
				if (_route == null) {
					_route = new Route (crud.logbook.routing);
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
		public TimeOfDay aout;
		public TimeOfDay ain;
		public Duration dur;
		public Duration night;
		public Duration inst;
		public Duration sim_inst;
		public bool hold;
		public string aprch = "";
		public bool xc;
		public int dland;
		public int nland;
		public string crew = "";
		public string notes = "";
		public string fltno = "";
		public TimeOfDay sout;
		public TimeOfDay sin;
		public Duration sdur;
		public string trip = "";
		public Date trip_date;

		public Flight ( FlightCrud crud ) {
			base (crud);
		}

		public override int bind_for_save (Statement stmt) {
			var i = 1;
			stmt.bind_int64				(i++, aircraft_id);
			stmt.bind_int64				(i++, role_id);
			stmt.bind_int64				(i++, dep_id);
			stmt.bind_int64				(i++, arr_id);
			stmt.bind_nonempty_text		(i++, date.to_iso8601 ());
			stmt.bind_int				(i++, leg);

			bind_time_of_day (stmt, ref i, aout, dep.timezone);
			bind_time_of_day (stmt, ref i, aout, Timezone ("UTC"));
			bind_time_of_day (stmt, ref i, ain, arr.timezone);
			bind_time_of_day (stmt, ref i, ain, Timezone ("UTC"));

			bind_duration (stmt, ref i, dur);
			bind_duration (stmt, ref i, night);
			bind_duration (stmt, ref i, inst);
			bind_duration (stmt, ref i, sim_inst);

			stmt.bind_int				(i++, (int) hold);
			stmt.bind_nonempty_text		(i++, aprch);
			stmt.bind_int				(i++, (int) xc);
			stmt.bind_int				(i++, dland);
			stmt.bind_int				(i++, nland);
			stmt.bind_nonempty_text		(i++, crew);
			stmt.bind_nonempty_text		(i++, notes);
			stmt.bind_nonempty_text		(i++, fltno);

			bind_time_of_day (stmt, ref i, sout, dep.timezone);
			bind_time_of_day (stmt, ref i, sout, Timezone ("UTC"));
			bind_time_of_day (stmt, ref i, sin, arr.timezone);
			bind_time_of_day (stmt, ref i, sin, Timezone ("UTC"));

			bind_duration (stmt, ref i, sdur);

			stmt.bind_nonempty_text		(i++, trip);
			stmt.bind_nonempty_text		(i++, trip_date.to_iso8601() );
			return i;
		}

		private void bind_time_of_day (Statement stmt, ref int iter, TimeOfDay tod, Timezone in_timezone) {
			if (tod.valid ()) {
				stmt.bind_text (iter++, ((Datetime (date, tod).in_timezone (in_timezone)).time_of_day).to_iso8601 ());
			} else {
				stmt.bind_null (iter++);
			}
		}

		private void bind_duration (Statement stmt, ref int iter, Duration dur) {
			if (dur.to_minutes () > 0) {
				stmt.bind_int64 (iter++, dur.to_minutes ());
			} else {
				stmt.bind_null (iter++);
			}
		}

		public override void set_from_stmt (Statement stmt) {
			var i = 1;
			aircraft_id = stmt.column_int64 (i++);
			role_id     = stmt.column_int64 (i++);
			dep_id      = stmt.column_int64 (i++);
			arr_id      = stmt.column_int64 (i++);
			date        = Date.from_iso8601 (stmt.column_text (i++));
			leg         = stmt.column_int   (i++);
			i++; // skip local ss read
			aout        = TimeOfDay.from_tzname_time ("UTC", stmt.column_text (i++));
			i++; // skip local
			ain         = TimeOfDay.from_tzname_time ("UTC", stmt.column_text (i++));
			dur         = Duration.from_minutes (stmt.column_int64 (i++));
			night       = Duration.from_minutes (stmt.column_int64 (i++));
			inst        = Duration.from_minutes (stmt.column_int64 (i++));
			sim_inst    = Duration.from_minutes (stmt.column_int64 (i++));
			hold        = (bool) stmt.column_int (i++);
			aprch       = stmt.column_text (i++);
			xc          = (bool) stmt.column_int (i++);
			dland       = stmt.column_int (i++);
			nland       = stmt.column_int (i++);
			crew        = stmt.column_text (i++);
			notes       = stmt.column_text (i++);
			fltno       = stmt.column_text (i++);
			i++; // skip local
			sout        = TimeOfDay.from_tzname_time ("UTC", stmt.column_text (i++));
			i++; // skip local
			sin         = TimeOfDay.from_tzname_time ("UTC", stmt.column_text (i++));
			sdur        = Duration.from_minutes (stmt.column_int64 (i++));
			trip        = stmt.column_text (i++);
			trip_date   = Date.from_iso8601 (stmt.column_text (i++));
//			stderr.printf("WTF: %s\n", sout.timezone.name);
		}

		protected override bool save_dependencies () {
			if (aircraft != null && aircraft.save ()) aircraft_id = aircraft.id;
			if (role != null && role.save ()) role_id = role.id;
			if (dep != null && dep.save ()) dep_id = dep.id;
			if (arr != null && arr.save ()) arr_id = arr.id;
			return true;
		}

		protected override bool save_dependents () {
			route.save ();
			return true;
		}
	}
}