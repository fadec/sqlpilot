using Sqlite;
namespace Sqlp {
	public class ModelTaggingTable : Table <Logbook, ModelTagging> {

		public ModelTaggingTable (Logbook logbook) {
			this.record_type = typeof (ModelTagging);
			this.database = logbook;
			this.table_name = "ModelTaggings";
		}

	}
}
