using Sqlite;
namespace Sqlp {
	public class ModelTable : Table <Logbook, Model> {

		private Statement find_by_abbreviation_stmt;
		private Statement find_tags_for_id_stmt;

		public ModelTable ( Logbook logbook ) {
			this.record_type = typeof (Model);
			this.database = logbook;
			this.table_name = "Models";
		}

		construct {
			var find_by_abbreviation_sql = "SELECT * FROM Models WHERE Abbreviation = ?;";
			find_by_abbreviation_stmt = database.prepare_statement (find_by_abbreviation_sql);
			var find_tags_for_id_sql = "SELECT Name from ModelTaggings LEFT JOIN ModelTags ON ModelTags.id = model_tag_id WHERE model_id = ?";
			find_tags_for_id_stmt = database.prepare_statement (find_tags_for_id_sql);
		}

		public Model? find_by_abbreviation (string abbreviation) {
			find_by_abbreviation_stmt.bind_text (1, abbreviation);
			return (find_first (find_by_abbreviation_stmt) as Model);
		}


	}
}