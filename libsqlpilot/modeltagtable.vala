using GLib;
using Sqlite;

namespace Sqlp {
	public class ModelTagTable : Table {

		public ModelTagTable (Logbook logbook) {
			this.record_type = typeof (Tag);
			this.database = logbook;
			this.table_name = "ModelProperties";
		}

	}
}
