using Sqlite;
namespace Sqlp {
	public class Takeoff : Record {

		private int64 flight_id;
		private Flight? _flight;
		public Flight? flight {
			get {
				if (_flight == null && flight_id != 0) {
					_flight = (table.database as Logbook).flight.find_by_id (flight_id) as Flight?;
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
					_airport = (table.database as Logbook).airport.find_by_id (airport_id) as Airport?;
				}
				return _airport;
			}
			set {
				_airport = value;
				airport_id = (value == null) ? 0 : value.id;
			}
		}

		private int64 surface_id;
		private Surface? _surface;
		public Surface? surface {
			get {
				if (_surface == null && surface_id != 0) {
					_surface = (table.database as Logbook).surfaces.find_by_id (surface_id) as Surface?;
				}
				return _surface;
			}
			set {
				_surface = value;
				surface_id = (value == null) ? 0 : value.id;
			}
		}

		public int sequence { get; set; }
		public bool night { get; set; }
		public string runway { get; set; }
		public double crosswind { get; set; }
		public double visibility { get; set; }
		public bool aborted { get; set; }

		protected override int bind_for_save (Statement stmt) {
			var i = 1;
			stmt.bind_nonzero_int64 (i++, flight_id);
			stmt.bind_nonzero_int64 (i++, airport_id);
			stmt.bind_nonzero_int64 (i++, surface_id);
			stmt.bind_int   (i++, sequence);
			stmt.bind_int (i++, (int) night);
			stmt.bind_nonempty_text (i++, runway);
			stmt.bind_double (i++, crosswind);
			stmt.bind_double (i++, visibility);
			stmt.bind_int (i++, (int) aborted);
			return i;
		}

		protected override void set_from_stmt (Statement stmt) {
			var i = 1;
			flight_id = stmt.column_int64 (i++);
			airport_id = stmt.column_int64 (i++);
			surface_id = stmt.column_int (i++);
			sequence = stmt.column_int (i++);
		    night = (bool) stmt.column_int (i++);
			runway = empty_if_null (stmt.column_text (i++));
			crosswind = stmt.column_double (i++);
			visibility = stmt.column_double (i++);
			aborted = (bool) stmt.column_int (i++);
		}

	}
}
