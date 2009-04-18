using Sqlite;
namespace Sqlp {
	public class ApproachTable : Table <Logbook, Approach> {
		
		private Statement find_by_flight_id_stmt;
		private Statement find_by_approach_type_id_stmt;

		public ApproachTable (Logbook logbook) {
			this.record_type = typeof (Approach);
			this.database = logbook;
			this.table_name = "Approaches";
		}

		construct {
			find_by_flight_id_stmt = database.prepare_statement ("SELECT * FROM Approaches WHERE flight_id = ?");
			find_by_approach_type_id_stmt = database.prepare_statement ("SELECT * FROM Approaches WHERE approach_type_id = ?");
		}


		public List <Approach> find_by_flight_id (int64 flight_id) {
			weak Statement stmt = find_by_flight_id_stmt;
			stmt.bind_nonzero_int64 (1, flight_id);
			return find_all (stmt);
		}

		public List <Approach> find_by_approach_type_id (int64 approach_type_id) {
			weak Statement stmt = find_by_approach_type_id_stmt;
			stmt.bind_nonzero_int64 (1, approach_type_id);
			return find_all (stmt);
		}


	}
}
