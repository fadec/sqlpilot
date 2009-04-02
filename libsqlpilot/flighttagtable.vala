using GLib;
using Sqlite;

namespace Sqlp {
	public class FlightTagTable : TagTable {

		public FlightTagTable (Logbook logbook) {
			this.record_type = typeof (Tag);
			this.database = logbook;
			this.table_name = "FlightTags";
		}

	}
}
