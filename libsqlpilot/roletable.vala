using Sqlite;
namespace Sqlp {
	public class RoleTable : Table {
		
		private Statement find_by_abbreviation_stmt;
		public Statement unique_abbreviation_stmt;

		public RoleTable (Logbook logbook) {
			this.record_type = typeof (Role);
			this.database = logbook;
			this.table_name = "Roles";
		}

		construct {
			var find_by_abbreviation_sql = "SELECT * FROM Roles WHERE Abbreviation = ?;";
			find_by_abbreviation_stmt = database.prepare_statement (find_by_abbreviation_sql);
			unique_abbreviation_stmt = prepare_unique_column_statement ("Abbreviation");
		}

		public Role? find_by_abbreviation (string abbreviation) {
			find_by_abbreviation_stmt.bind_text (1, abbreviation);
			return find_first (find_by_abbreviation_stmt) as Role?;
		}

		public Role find_or_new_by_abbreviation (string ident) {
			var role = find_by_abbreviation (ident);
			if (role != null) return role;
			role = new_record () as Role;
			role.abbreviation = ident;
			return role;
		}
	}
}
