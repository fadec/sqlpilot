using Sqlite;
namespace Sqlp {
	public class ModelTable : Table <Model> {

		private Statement find_by_ident_stmt;

		public ModelTable ( Logbook logbook ) {
			this.record_type = typeof (Model);
			this.logbook = logbook;
			this.table_name = "Models";
		}

		construct {
			var find_by_ident_sql = "SELECT * FROM Models WHERE Abbreviation = ?;";
			find_by_ident_stmt = logbook.prepare_statement (find_by_ident_sql);
		}

		public Model? find_by_ident (string ident) {
			find_by_ident_stmt.bind_text (1, ident);
			return (find_first (find_by_ident_stmt) as Model);
		}


	}
}