using Sqlite;
namespace Sqlp {
	public class HoldTable : Table {
		
		private Statement find_by_flight_id_stmt;

		public HoldTable (Logbook logbook) {
			this.record_type = typeof (Hold);
			this.database = logbook;
			this.table_name = "Holds";
		}

		construct {
			find_by_flight_id_stmt = database.prepare_statement ("SELECT * FROM Holds WHERE flight_id = ?");
		}


		public List <Flight> find_by_flight_id (int64 flight_id) {
			weak Statement stmt = find_by_flight_id_stmt;
			stmt.bind_nonzero_int64 (1, flight_id);
			return find_all (stmt);
		}


	}
}
