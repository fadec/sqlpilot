using GLib;
using Sqlite;
namespace Sqlp {
	public class RoutingTable : Table <Routing> {
		protected Statement find_by_flight_id_stmt;
		protected Statement destroy_by_flight_id_stmt;

		private Routing dummy_for_header_inclusion_delete_me_later_sorry_im_here {get; set;}

		public RoutingTable ( Logbook logbook ) {
			this.record_type = typeof (Routing);
			this.logbook = logbook;
			this.table_name = "Routing";
		}

		construct {
			var find_by_flight_id_sql = "SELECT * FROM Routing WHERE flight_id = ?;";
			find_by_flight_id_stmt = logbook.prepare_statement (find_by_flight_id_sql);

			var destroy_by_flight_id_sql = "DELETE FROM Routing WHERE flight_id = ?;";
			destroy_by_flight_id_stmt = logbook.prepare_statement (destroy_by_flight_id_sql);
		}

		public List<Routing> find_by_flight ( Flight f ) {
			find_by_flight_id_stmt.bind_int64 (1, f.id);
			return find_all (find_by_flight_id_stmt);
		}


	}
}