using Sqlite;
namespace SqlPilot {
	public class RoleCrud : Crud {

		public RoleCrud ( Logbook logbook ) {
			var find_sql = "SELECT * FROM Roles WHERE id = ?;";

			var insert_sql = "INSERT INTO Roles" +
			" (ident, name, pic, sic, fe, solo, dual, instruct, total)" +
			" VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);";

			var update_sql = "UPDATE Roles" +
			" SET ident = ?, name = ?, pic = ?, sic = ?, fe = ?, solo = ?, dual = ?, instruct = ?, total = ?" +
				" WHERE id = ?;";

			var destroy_sql = "DELETE FROM Roles WHERE id = ?;";

			init_crud_statements (logbook, find_sql, insert_sql, update_sql, destroy_sql);
		}

		public override Record new_record () {
			return new Role ( this ) as Record;
		}

		public Role? find_first (Statement stmt) {
			return (record_find_first (stmt) as Role);
		}
		
 		public Role? find_by_id (int64 id) {
			return (record_find_by_id (id) as Role);
 		}



	}
}