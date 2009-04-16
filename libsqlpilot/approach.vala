using Sqlite;
namespace Sqlp {
	public class Approach : Record <ApproachTable> {

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

		private int64 landing_id;
		private Landing? _landing;
		public Landing? landing {
			get {
				if (_landing == null && landing_id != 0) {
					_landing = table.database.landings.find_by_id (landing_id);
				}
				return _landing;
			}
			set {
				_landing = value;
				landing_id = (value == null) ? 0 : value.id;
			}
		}

		public int sequence { get; set; }
		public string runway { get; set; }
		public double visibility { get; set; }
		public double ceiling { get; set; }
		public bool coupled { get; set; }

		protected override int bind_for_save (Statement stmt) {
			var i = 1;
			stmt.bind_nonzero_int64 (i++, flight_id);
			stmt.bind_nonzero_int64 (i++, airport_id);
			stmt.bind_nonzero_int64 (i++, approach_type_id);
			stmt.bind_nonzero_int64 (i++, landing_id);
			stmt.bind_int   (i++, sequence);
			stmt.bind_nonempty_text (i++, runway);
			stmt.bind_double (i++, visibility);
			stmt.bind_double (i++, ceiling);
			stmt.bind_int (i++, (int) coupled);
			return i;
		}

		protected override void set_from_stmt (Statement stmt) {
			var i = 1;
			flight_id = stmt.column_int64 (i++);
			airport_id = stmt.column_int64 (i++);
			approach_type_id = stmt.column_int64 (i++);
			landing_id = stmt.column_int64 (i++);
			sequence = stmt.column_int (i++);
			runway = empty_if_null (stmt.column_text (i++));
			visibility = stmt.column_double (i++);
		    ceiling = stmt.column_double (i++);
			coupled = (bool) stmt.column_int (i++);
		}

	}
}
