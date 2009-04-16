using Sqlite;
namespace Sqlp {
	public class Landing : Record <LandingTable> {

		private int64 flight_id;
		private Flight? _flight;
		public Flight? flight {
			get {
				if (_flight == null && flight_id != 0) {
					_flight = table.database.flight.find_by_id (flight_id);
				}
				return _flight;
			}
			set {
				_flight = value;
				flight_id = (value == null) ? 0 : value.id;
			}
		}

		private int64 airport_id;
		private Airport? _airport;
		public Airport? airport {
			get {
				if (_airport == null && airport_id != 0) {
					_airport = table.database.airport.find_by_id (airport_id);
				}
				return _airport;
			}
			set {
				_airport = value;
				airport_id = (value == null) ? 0 : value.id;
			}
		}

		public int sequence { get; set; default = 0; }

		private int64 approach_type_id;
		private ApproachType? _approach_type;
		public ApproachType? approach_type {
			get {
				if (_approach_type == null && approach_type_id != 0) {
					_approach_type = table.database.approach_types.find_by_id (approach_type_id);
				}
				return _approach_type;
			}
			set {
				_approach_type = value;
				approach_type_id = (value == null) ? 0 : value.id;
			}
		}

		public string approach_runway { get; set; }
		public double visibility { get; set; }
		public double ceiling { get; set; }
		public bool coupled { get; set; }

		private int64 surface_id;
		private Surface? _surface;
		public Surface? surface {
			get {
				if (_surface == null && surface_id != 0) {
					_surface = table.database.surfaces.find_by_id (surface_id);
				}
				return _surface;
			}
			set {
				_surface = value;
				surface_id = (value == null) ? 0 : value.id;
			}
		}

		public bool night { get; set; }
		public string landing_runway { get; set; }
		public double crosswind { get; set; }
		public bool full_stop { get; set; }
		public bool night_vision { get; set; }
		public bool autoland { get; set; }

		protected override int bind_for_save (Statement stmt) {
			var i = 1;
			stmt.bind_nonzero_int64 (i++, flight_id);
			stmt.bind_nonzero_int64 (i++, airport_id);
			stmt.bind_int           (i++, sequence);
			if (has_approach ()) {
				stmt.bind_nonzero_int64 (i++, approach_type_id);
				stmt.bind_nonempty_text (i++, approach_runway);
				stmt.bind_double        (i++, visibility);
				stmt.bind_double        (i++, ceiling);
				stmt.bind_int           (i++, (int) coupled);
			} else {
				i += 5;
			}
			if (has_landing ()) {
				stmt.bind_nonzero_int64 (i++, surface_id);
				stmt.bind_int           (i++, (int) night);
				stmt.bind_nonempty_text (i++, landing_runway);
				stmt.bind_double        (i++, crosswind);
				stmt.bind_int           (i++, (int) full_stop);
				stmt.bind_int           (i++, (int) night_vision);
				stmt.bind_int           (i++, (int) autoland);
			} else {
				i += 7;
			}
			return i;
		}

		protected override void set_from_stmt (Statement stmt) {
			var i = 1;
			flight_id = stmt.column_int64 (i++);
			airport_id = stmt.column_int64 (i++);
			sequence = stmt.column_int (i++);
			approach_type_id = stmt.column_int64 (i++);
			approach_runway = empty_if_null (stmt.column_text (i++));
			visibility = stmt.column_double (i++);
		    ceiling = stmt.column_double (i++);
			coupled = (bool) stmt.column_int (i++);
			surface_id = stmt.column_int (i++);
		    night = (bool) stmt.column_int (i++);
			landing_runway = empty_if_null (stmt.column_text (i++));
			crosswind = stmt.column_double (i++);
		    full_stop = (bool) stmt.column_int (i++);
			night_vision = (bool) stmt.column_int (i++);
			autoland = (bool) stmt.column_int (i++);
		}

		public bool has_approach () {
			return (approach_type != null);
		}

		public bool has_landing () {
			return (surface != null);
		}

	}
}
