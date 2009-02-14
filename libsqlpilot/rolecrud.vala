using Sqlite;
namespace Sqlp {
	public class RoleCrud : Crud {
		
		private Statement find_by_ident_stmt;
		
		public RoleCrud ( Logbook logbook ) {
			base (logbook, "Roles");
			var find_by_ident_sql = "SELECT * FROM Roles WHERE Ident = ?;";
			find_by_ident_stmt = logbook.prepare_statement (find_by_ident_sql);
		}
		
		public override Record new_record () {
			return new Role ( this );
		}
		public Role beget () { return new_record () as Role; }
		
 		public Role? find_by_id (int64 id) {
			return (record_find_by_id (id) as Role);
 		}

		public Role? find_by_ident (string ident) {
			find_by_ident_stmt.bind_text (1, ident);
			return (record_find_first (find_by_ident_stmt) as Role);
		}

	}
}