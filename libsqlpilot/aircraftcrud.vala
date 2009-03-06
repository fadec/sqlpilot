using Sqlite;

namespace Sqlp {
	public class AircraftCrud : Crud <Aircraft> {

		Statement find_by_tail_stmt;
		public AircraftCrud ( Logbook logbook ) {
			this.record_type = typeof (Aircraft);
			this.logbook = logbook;
			this.table_name = "Aircraft";
		}

		construct {
			var find_by_tail_sql = "SELECT * FROM AIRCRAFT WHERE Tail = ?;";
			find_by_tail_stmt = logbook.prepare_statement (find_by_tail_sql);
		}

		public Aircraft? find_by_tail (string tail) {
			find_by_tail_stmt.bind_text (1, tail);
			return find_first (find_by_tail_stmt);
		}

	}
}