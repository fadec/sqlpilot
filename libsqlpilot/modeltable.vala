using Sqlite;
namespace Sqlp {
	public class ModelTable : Table <Logbook, Model> {

		private Statement find_by_abbreviation_stmt;

		public ModelTable ( Logbook logbook ) {
			this.record_type = typeof (Model);
			this.database = logbook;
			this.table_name = "Models";
		}

		construct {
			var find_by_abbreviation_sql = "SELECT * FROM Models WHERE Abbreviation = ?;";
			find_by_abbreviation_stmt = database.prepare_statement (find_by_abbreviation_sql);
		}

		public Model? find_by_abbreviation (string abbreviation) {
			find_by_abbreviation_stmt.bind_text (1, abbreviation);
			return (find_first (find_by_abbreviation_stmt) as Model);
		}


	}
}