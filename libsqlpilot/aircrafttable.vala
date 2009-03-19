using Sqlite;

namespace Sqlp {
	public class AircraftTable : Table <Logbook, Aircraft> {

		Statement find_by_tail_stmt;
		public AircraftTable ( Logbook logbook ) {
			this.record_type = typeof (Aircraft);
			this.database = logbook;
			this.table_name = "Aircraft";
		}

		construct {
			var find_by_tail_sql = "SELECT * FROM AIRCRAFT WHERE Tail = ?;";
			find_by_tail_stmt = database.prepare_statement (find_by_tail_sql);
		}

		public Aircraft? find_by_tail (string tail) {
			find_by_tail_stmt.bind_text (1, tail);
			return find_first (find_by_tail_stmt);
		}

	}
}