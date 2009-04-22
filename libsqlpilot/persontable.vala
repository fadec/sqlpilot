using GLib;
using Sqlite;

namespace Sqlp {
	public class PersonTable : Table {

		public PersonTable (Logbook logbook) {
			this.record_type = typeof (Person);
			this.database = logbook;
			this.table_name = "People";
		}

	}
}
