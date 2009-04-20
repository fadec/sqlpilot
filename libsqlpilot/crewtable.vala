
namespace Sqlp {
	public class CrewTable : Table, ITaggingTable {

		public string object_id_column_name { get { return "flight_id"; } }
		public string tag_id_column_name { get { return "person_id"; } }

		public CrewTable (Logbook logbook) {
			this.record_type = typeof (Crew);
			this.database = logbook;
			this.table_name = "Crew";
		}

	}
}
