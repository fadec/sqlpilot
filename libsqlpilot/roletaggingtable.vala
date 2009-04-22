using Sqlite;
namespace Sqlp {
	public class RoleTaggingTable : Table, IJoinTable {

		public string parent_id_column_name { get { return "role_id"; } }
		public string child_id_column_name { get { return "role_property_id"; } }

		public RoleTaggingTable (Logbook logbook) {
			this.record_type = typeof (Tagging);
			this.database = logbook;
			this.table_name = "RolePropertyAssignments";
		}

	}
}
