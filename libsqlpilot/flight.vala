using Sqlite;
using Stardate;

namespace SqlPilot {
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
				if (value == null) return;
				aircraft_id = value.id;
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
				if (value == null) return;
				role_id = value.id;
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
				if (value == null) return;
				dep_id = value.id;
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
				if (value == null) return;
				arr_id = value.id;
			}
		}


		private Route? _route;
		public Route route {
			get {
				if (_route == null) {
					_route = new Route (crud.logbook.routing);
					_route.flight = this;
				}
				return _route;
			}
			set {
				if (value == null) _route.clear;
				else _route = value;
			}
		}

		public Stardate.Date date;
		public int leg;
		public TimeOfDay? aout;
		public TimeOfDay? ain;
		public Duration? dur;
		public Duration? night;
		public Duration? inst;
		public Duration? sim_inst;
		public bool hold;
		public string aprch = "";
		public bool xc;
		public int dland;
		public int nland;
		public string crew = "";
		public string notes = "";
		public string fltno = "";
		public TimeOfDay? sout;
		public TimeOfDay? sin;
		public Duration? sdur;
		public string trip = "";
		public Stardate.Date trip_date;

		public Flight ( FlightCrud crud ) {
			base (crud);
		}

		public override int bind_for_save (Statement stmt) {
			var i = 1;
			stmt.bind_int64 (i++, aircraft_id);
			stmt.bind_int64 (i++, role_id);
			stmt.bind_int64 (i++, dep_id);
			stmt.bind_int64 (i++, arr_id);
			stmt.bind_text  (i++, date.to_iso8601 ());
			stmt.bind_int   (i++, leg);
			stmt.bind_text  (i++, aout.to_iso8601 ());
			stmt.bind_text  (i++, aout.to_utc ().to_iso8601 ());
			stmt.bind_text  (i++, ain.to_iso8601 ());
			stmt.bind_text  (i++, ain.to_utc ().to_iso8601 ());
			stmt.bind_int64 (i++, dur.to_seconds ());
			stmt.bind_int64 (i++, night.to_seconds ());
			stmt.bind_int64 (i++, inst.to_seconds ());
			stmt.bind_int64 (i++, sim_inst.to_seconds ());
			stmt.bind_int   (i++, (int) hold);
			stmt.bind_text  (i++, aprch);
			stmt.bind_int   (i++, (int) xc);
			stmt.bind_int   (i++, dland);
			stmt.bind_int   (i++, nland);
			stmt.bind_text  (i++, crew);
			stmt.bind_text  (i++, notes);
			stmt.bind_text  (i++, fltno);
			stmt.bind_text  (i++, sout.to_iso8601 ());
			stmt.bind_text  (i++, sout.to_utc ().to_iso8601 ());
			stmt.bind_text  (i++, sin.to_iso8601 ());
			stmt.bind_text  (i++, sin.to_utc ().to_iso8601 ());
			stmt.bind_int64 (i++, sdur.to_seconds ());
			stmt.bind_text  (i++, trip);
			stmt.bind_text  (i++, trip_date.to_iso8601() );
			return i;
		}

		public override void set_from_stmt (Statement stmt) {
			var i = 1;
			aircraft_id = stmt.column_int64 (i++);
			role_id     = stmt.column_int64 (i++);
			dep_id      = stmt.column_int64 (i++);
			arr_id      = stmt.column_int64 (i++);
			date        = Stardate.Date ().from_iso8601 (stmt.column_text (i++));
			leg         = stmt.column_int   (i++);
			i++; // skip local
			aout        = TimeOfDay ().from_iso8601 (stmt.column_text (i++));
			i++;
			ain         = TimeOfDay ().from_iso8601 (stmt.column_text (i++));
			dur         = Duration ().from_seconds (stmt.column_int64 (i++));
			night       = Duration ().from_seconds (stmt.column_int64 (i++));
			inst        = Duration ().from_seconds (stmt.column_int64 (i++));
			sim_inst    = Duration ().from_seconds (stmt.column_int64 (i++));
			hold        = (bool) stmt.column_int (i++);
			aprch       = stmt.column_text (i++);
			xc          = (bool) stmt.column_int (i++);
			dland       = stmt.column_int (i++);
			nland       = stmt.column_int (i++);
			crew        = stmt.column_text (i++);
			notes       = stmt.column_text (i++);
			fltno       = stmt.column_text (i++);
			i++; // skip local
			sout        = TimeOfDay ().from_iso8601 (stmt.column_text (i++));
			i++;
			sin         = TimeOfDay ().from_iso8601 (stmt.column_text (i++));
			sdur        = Duration ().from_seconds (stmt.column_int64 (i++));
			trip        = stmt.column_text (i++);
			trip_date   = Stardate.Date ().from_iso8601 (stmt.column_text (i++));
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