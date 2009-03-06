using Sqlite;
namespace Sqlp {
	public class RoleCrud : Crud <Role> {
		
		private Statement find_by_abbreviation_stmt;
		
		public RoleCrud ( Logbook logbook ) {
			this.record_type = typeof (Role);
			this.logbook = logbook;
			this.table_name = "Roles";
		}

		construct {
			var find_by_abbreviation_sql = "SELECT * FROM Roles WHERE Abbreviation = ?;";
			find_by_abbreviation_stmt = logbook.prepare_statement (find_by_abbreviation_sql);
		}

		public Role? find_by_abbreviation (string abbreviation) {
			find_by_abbreviation_stmt.bind_text (1, abbreviation);
			return find_first (find_by_abbreviation_stmt);
		}

	}
}