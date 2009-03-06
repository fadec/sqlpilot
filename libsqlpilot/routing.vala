using Sqlite;
namespace Sqlp {
	public class Routing : Record {

		private int64 flight_id;
		private Flight? _flight;
		public Flight? flight {
			get {
				if (_flight == null && flight_id != 0) {
					_flight = crud.logbook.flight.find_by_id (flight_id);
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
					_airport = crud.logbook.airport.find_by_id (airport_id);
				}
				return _airport;
			}
			set {
				_airport = value;
				airport_id = (value == null) ? 0 : value.id;
			}
		}


		public int seq;

		public override int bind_for_save (Statement stmt) {
			var i = 1;
			stmt.bind_int64 (i++, flight_id);
			stmt.bind_int64 (i++, airport_id);
			stmt.bind_int   (i++, seq);
			return i;
		}

		public override void set_from_stmt (Statement stmt) {
			var i = 1;
			flight_id  = stmt.column_int64 (i++);
			airport_id = stmt.column_int64 (i++);
			seq        = stmt.column_int (i++);
		}

		protected override bool save_dependencies () {
			if (airport != null && airport.save ()) airport_id = airport.id;
			return true;
		}

		protected override bool save_dependents () {

			return true;
		}
	}
}