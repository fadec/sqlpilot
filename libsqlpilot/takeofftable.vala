using Sqlite;
namespace Sqlp {
	public class TakeoffTable : Table {
		
		private Statement find_by_flight_id_stmt;
		private Statement find_by_surface_id_stmt;
		private Statement find_by_sequence_stmt;
		private Statement find_greatest_sequence_stmt;

		public TakeoffTable (Logbook logbook) {
			this.record_type = typeof (Takeoff);
			this.database = logbook;
			this.table_name = "Takeoffs";
		}

		construct {
			find_by_flight_id_stmt = database.prepare_statement ("SELECT * FROM Takeoffs WHERE flight_id = ?");
			find_by_surface_id_stmt = database.prepare_statement ("SELECT * FROM Takeoffs WHERE surface_id = ?");
			find_by_sequence_stmt = database.prepare_statement ("SELECT * FROM Takeoffs WHERE Sequence = ?");
			find_greatest_sequence_stmt = database.prepare_statement ("SELECT Sequence FROM Takeoffs WHERE flight_id = ? ORDER BY Sequence DESC");
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

		public Takeoff? find_by_sequence (int sequence) {
			weak Statement stmt = find_by_sequence_stmt;
			stmt.bind_int (1, sequence);
			return find_first (stmt) as Takeoff?;
		}

		public int next_sequence (int64 flight_id) {
			weak Statement stmt = find_greatest_sequence_stmt;
			int next = 1;

			stmt.bind_int64 (1, flight_id);
			if (stmt.step () == ROW) {
				next = stmt.column_int (0) + 1;
			}
			stmt.reset ();
			stmt.clear_bindings ();

			return next;
		}

	}
}
