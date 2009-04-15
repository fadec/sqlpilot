using Sqlite;
namespace Sqlp {
	public class SurfaceTable : Table <Logbook, Surface> {
		
		private Statement find_by_abbreviation_stmt;
		public Statement unique_abbreviation_stmt;

		public SurfaceTable (Logbook logbook) {
			this.record_type = typeof (Surface);
			this.database = logbook;
			this.table_name = "Surfaces";
		}

		construct {
			var find_by_abbreviation_sql = "SELECT * FROM Surfaces WHERE Abbreviation = ?;";
			find_by_abbreviation_stmt = database.prepare_statement (find_by_abbreviation_sql);
			unique_abbreviation_stmt = prepare_unique_column_statement ("Abbreviation");
		}

		public Surface? find_by_abbreviation (string abbreviation) {
			find_by_abbreviation_stmt.bind_text (1, abbreviation);
			return find_first (find_by_abbreviation_stmt);
		}

		public Surface find_or_new_by_abbreviation (string ident) {
			var surface = find_by_abbreviation (ident);
			if (surface != null) return surface;
			surface = new_record ();
			surface.abbreviation = ident;
			return surface;
		}
	}
}
