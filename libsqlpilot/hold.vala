using Sqlite;
namespace Sqlp {
	public class Hold : Record {

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

		public int sequence { get; set; }
		public string location { get; set; }
		public int turns { get; set; }
		public Duration duration { get; set; }


		protected override int bind_for_save (Statement stmt) {
			var i = 1;
			stmt.bind_nonzero_int64 (i++, flight_id);
			stmt.bind_int   (i++, sequence);
			stmt.bind_nonempty_text (i++, location);
			stmt.bind_int (i++, turns);
			stmt.bind_nonzero_int64 (i++, duration.to_seconds ());
			return i;
		}

		protected override void set_from_stmt (Statement stmt) {
			var i = 1;
			flight_id = stmt.column_int64 (i++);
			sequence = stmt.column_int (i++);
			location = empty_if_null (stmt.column_text (i++));
			turns = stmt.column_int (i++);
			duration = Duration.from_seconds ((long) stmt.column_int64 (i++));
		}

	}
}
