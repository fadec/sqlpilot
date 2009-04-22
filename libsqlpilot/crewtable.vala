
namespace Sqlp {
	public class CrewTable : Table, IJoinTable {

		public string parent_id_column_name { get { return "flight_id"; } }
		public string child_id_column_name { get { return "person_id"; } }

		public CrewTable (Logbook logbook) {
			this.record_type = typeof (Crew);
			this.database = logbook;
			this.table_name = "Crew";
		}

	}
}
