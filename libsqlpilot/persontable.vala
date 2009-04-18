using GLib;
using Sqlite;

namespace Sqlp {
	public class PersonTable : TagTable {

		public PersonTable (Logbook logbook) {
			this.record_type = typeof (Tag);
			this.database = logbook;
			this.table_name = "People";
		}

	}
}
