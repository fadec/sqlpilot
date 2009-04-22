using Sqlite;
namespace Sqlp {
	public class FlightTaggingTable : Table, IJoinTable {

		public string parent_id_column_name { get { return "flight_id"; } }
		public string child_id_column_name { get { return "flight_tag_id";} }

		public FlightTaggingTable (Logbook logbook) {
			this.record_type = typeof (Tagging);
			this.database = logbook;
			this.table_name = "FlightTaggings";
		}

	}
}
