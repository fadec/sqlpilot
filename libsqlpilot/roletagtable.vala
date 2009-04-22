using GLib;
using Sqlite;

namespace Sqlp {
	public class RoleTagTable : Table {

		public RoleTagTable (Logbook logbook) {
			this.record_type = typeof (Tag);
			this.database = logbook;
			this.table_name = "RoleProperties";
		}

	}
}
