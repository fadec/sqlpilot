using Sqlite;

namespace Sqlp {
	public class AircraftTable : Table <Logbook, Aircraft> {

		Statement find_by_tail_stmt;
		Statement find_by_registration_stmt;

		public AircraftTable ( Logbook logbook ) {
			this.record_type = typeof (Aircraft);
			this.database = logbook;
			this.table_name = "Aircraft";
		}

		construct {
			var find_by_tail_sql = "SELECT * FROM AIRCRAFT WHERE Tail = ?;";
			find_by_tail_stmt = database.prepare_statement (find_by_tail_sql);
			var find_by_registration_sql = "SELECT * FROM AIRCRAFT WHERE Registration = ?;";
			find_by_registration_stmt = database.prepare_statement (find_by_registration_sql);
		}

		public Aircraft? find_by_tail (string tail) {
			find_by_tail_stmt.bind_text (1, tail);
			return find_first (find_by_tail_stmt);
		}

		public Aircraft? find_by_registration (string registration) {
			find_by_registration_stmt.bind_text (1, registration);
			return find_first (find_by_registration_stmt);
		}

		public Aircraft find_or_new_by_tail (string tail) {
			var aircraft = find_by_tail (tail);
			if (aircraft != null) return aircraft;
			aircraft = new_record ();
			aircraft.tail = tail;
			return aircraft;
		}

		public Aircraft find_or_new_by_registration (string registration) {
			var aircraft = find_by_registration (registration);
			if (aircraft != null) return aircraft;
			aircraft = new_record ();
			aircraft.registration = registration;
			return aircraft;
		}


	}
}