using Sqlite;
namespace Sqlp {
	public class ModelTaggingTable : Table, ITaggingTable {

		public string object_id_column_name { get { return "model_id"; } }
		public string tag_id_column_name { get { return "model_property_id"; } }

		public ModelTaggingTable (Logbook logbook) {
			this.record_type = typeof (Tagging);
			this.database = logbook;
			this.table_name = "ModelPropertyAssignments";
// 			this.object_id_column_name = "model_id";
// 			this.tag_id_column_name = "model_property_id";
		}
	}
}
