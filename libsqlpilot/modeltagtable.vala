using Sqlite;
namespace Sqlp {
	public class ModelTagTable : Table <Logbook, ModelTag> {

		public ModelTagTable (Logbook logbook) {
			this.record_type = typeof (ModelTag);
			this.database = logbook;
			this.table_name = "ModelTags";
		}

	}
}
