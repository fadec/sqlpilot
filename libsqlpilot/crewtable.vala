
namespace Sqlp {
	public class CrewTable : TaggingTable {

		public CrewTable (Logbook logbook) {
			this.record_type = typeof (Tagging);
			this.database = logbook;
			this.table_name = "Crew";
			this.object_id_column_name = "flight_id";
			this.tag_id_column_name = "person_id";
		}

	}
}
