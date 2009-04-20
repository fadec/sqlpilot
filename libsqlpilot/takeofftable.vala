using Sqlite;
namespace Sqlp {
	public class TakeoffTable : Table {
		
		private Statement find_by_flight_id_stmt;
		private Statement find_by_surface_id_stmt;

		public TakeoffTable (Logbook logbook) {
			this.record_type = typeof (Takeoff);
			this.database = logbook;
			this.table_name = "Takeoffs";
		}

		construct {
			find_by_flight_id_stmt = database.prepare_statement ("SELECT * FROM Takeoffs WHERE flight_id = ?");
			find_by_surface_id_stmt = database.prepare_statement ("SELECT * FROM Landings WHERE surface_id = ?");
		}


		public List <Takeoff> find_by_flight_id (int64 flight_id) {
			weak Statement stmt = find_by_flight_id_stmt;
			stmt.bind_nonzero_int64 (1, flight_id);
			return find_all (stmt);
		}


		public List <Takeoff> find_by_surface_id (int64 surface_id) {
			weak Statement stmt = find_by_surface_id_stmt;
			stmt.bind_nonzero_int64 (1, surface_id);
			return find_all (stmt);
		}


	}
}
