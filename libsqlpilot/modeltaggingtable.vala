using Sqlite;
namespace Sqlp {
	public class ModelTaggingTable : TaggingTable {

		public ModelTaggingTable (Logbook logbook) {
			this.record_type = typeof (Tagging);
			this.database = logbook;
			this.table_name = "ModelTaggings";
			this.object_id_column_name = "model_id";
			this.tag_id_column_name = "model_tag_id";
		}

	}
}
