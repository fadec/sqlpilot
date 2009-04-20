using Sqlite;
namespace Sqlp {
	public class Glide : Record {

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

		private int64 launch_type_id;
		private LaunchType? _launch_type;
		public LaunchType? launch_type {
			get {
				if (_launch_type == null && launch_type_id != 0) {
					_launch_type = (table.database as Logbook).launch_types.find_by_id (launch_type_id) as LaunchType?;
				}
				return _launch_type;
			}
			set {
				_launch_type = value;
				launch_type_id = (value == null) ? 0 : value.id;
			}
		}

		public int sequence { get; set; }
		public Duration duration { get; set; }
		public double distance { get; set; }
		public double release_altitude { get; set; }
		public double engine_start_altitude { get; set; }
		public double max_altitude { get; set; }

		protected override int bind_for_save (Statement stmt) {
			var i = 1;
			stmt.bind_nonzero_int64 (i++, flight_id);
			stmt.bind_nonzero_int64 (i++, launch_type_id);
			stmt.bind_int   (i++, sequence);
			stmt.bind_nonzero_int64   (i++, duration.to_seconds ());
			stmt.bind_double (i++, distance);
			stmt.bind_double (i++, release_altitude);
			stmt.bind_double (i++, max_altitude);
			stmt.bind_double (i++, engine_start_altitude);
			return i;
		}

		protected override void set_from_stmt (Statement stmt) {
			var i = 1;
			flight_id = stmt.column_int64 (i++);
			launch_type_id = stmt.column_int64 (i++);
			sequence = stmt.column_int (i++);
			duration = Duration.from_seconds ((long)stmt.column_int64 (i++));
			distance = stmt.column_double (i++);
			release_altitude = stmt.column_double (i++);
			max_altitude = stmt.column_double (i++);
			engine_start_altitude = stmt.column_double (i++);
		}

	}
}
