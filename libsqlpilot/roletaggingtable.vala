using Sqlite;
namespace Sqlp {
	public class RoleTaggingTable : TaggingTable {

		public RoleTaggingTable (Logbook logbook) {
			this.record_type = typeof (Tagging);
			this.database = logbook;
			this.table_name = "RoleTaggings";
			this.object_id_column_name = "role_id";
			this.tag_id_column_name = "role_tag_id";
		}

	}
}
