using GLib;
using Sqlite;

namespace Sqlp {
	public class RoutingTable : Table {
		protected Statement find_by_flight_id_stmt;
		protected Statement delete_by_flight_id_stmt;

		private Routing dummy_for_header_inclusion_delete_me_later_sorry_im_here {get; set;}

		public RoutingTable ( Logbook logbook ) {
			this.record_type = typeof (Routing);
			this.database = logbook;
			this.table_name = "Routing";
		}

		construct {
			var find_by_flight_id_sql = "SELECT * FROM Routing WHERE flight_id = ?;";
			find_by_flight_id_stmt = database.prepare_statement (find_by_flight_id_sql);

			var delete_by_flight_id_sql = "DELETE FROM Routing WHERE flight_id = ?;";
			delete_by_flight_id_stmt = database.prepare_statement (delete_by_flight_id_sql);
		}

		public List<Routing> find_by_flight ( Flight f ) {
			find_by_flight_id_stmt.bind_nonzero_int64 (1, f.id);
			return find_all (find_by_flight_id_stmt);
		}

		public void delete_by_flight_id (int64 flight_id) {
			delete_by_flight_id_stmt.bind_int64 (1, flight_id);
			int err_code = delete_by_flight_id_stmt.step ();
			if (err_code != Sqlite.DONE) {
				warning ("Database error deleting %s with flight_id = %s\n%d: %s\n",
						 table_name,
						 flight_id.to_string (),
						 err_code,
						 delete_by_flight_id_stmt.db_handle().errmsg ());

			}
			delete_by_flight_id_stmt.reset ();
			delete_by_flight_id_stmt.clear_bindings ();
		}


	}
}