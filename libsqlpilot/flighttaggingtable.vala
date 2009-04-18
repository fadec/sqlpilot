using Sqlite;
namespace Sqlp {
	public class FlightTaggingTable : TaggingTable {

		public FlightTaggingTable (Logbook logbook) {
			this.record_type = typeof (Tagging);
			this.database = logbook;
			this.table_name = "FlightPropertyAssignments";
			this.object_id_column_name = "flight_id";
			this.tag_id_column_name = "flight_property_id";
		}

	}
}
