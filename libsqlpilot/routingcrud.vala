using Sqlite;
namespace SqlPilot {
	public class RoutingCrud : Crud {
		protected Statement find_by_flight_id_stmt;

		public RoutingCrud ( Logbook logbook ) {
			var find_sql = "SELECT * FROM Routing WHERE id = ?;";

			var insert_sql = "";

			var update_sql = "";

			var destroy_sql = "";

			init_crud_statements (logbook, find_sql, insert_sql, update_sql, destroy_sql);

			var find_by_flight_id_sql = "SELECT * FROM Routing WHERE flight_id = ?;";
			find_by_flight_id_stmt = logbook.prepare_statement (find_by_flight_id_sql);
			
		}

		public override Record new_record () {
			return new Routing ( this ) as Record;
		}

		public List<Routing> find_by_flight ( Flight f ) {
			find_by_flight_id_stmt.bind_int64 (1, f.id);
			return record_find_all (find_by_flight_id_stmt); // as List<Routing>
		}


	}
}