using Sqlite;
namespace SqlPilot {
	public class RoutingCrud : Crud {
		protected Statement find_by_flight_id_stmt;
		protected Statement destroy_by_flight_id_stmt;

		public RoutingCrud ( Logbook logbook ) {
			base (logbook, "Routing");

			var find_by_flight_id_sql = "SELECT * FROM Routing WHERE flight_id = ?;";
			find_by_flight_id_stmt = logbook.prepare_statement (find_by_flight_id_sql);

			var destroy_by_flight_id_sql = "DELETE FROM Routing WHERE flight_id = ?;";
			destroy_by_flight_id_stmt = logbook.prepare_statement (destroy_by_flight_id_sql);
		}

		public override Record new_record () {
			return new Routing ( this ) as Record;
		}
		public Routing beget () { return new_record () as Routing; }

		public List<Routing> find_by_flight ( Flight f ) {
			find_by_flight_id_stmt.bind_int64 (1, f.id);
			return record_find_all (find_by_flight_id_stmt); // as List<Routing>
		}


	}
}