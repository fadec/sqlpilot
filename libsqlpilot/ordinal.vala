using GLib;
using Sqlite;

namespace Sqlp {
	public struct Ordinal {
	
		public int value;
		
		public Ordinal () {
			value = -1;
		}

		public Ordinal.set (int val) {
			value = val < 0 ? -1 : val;
		}

		public Ordinal.from_stmt (Statement stmt, int offset) {
			if (stmt.column_type (offset) == Sqlite.NULL)
				value = -1;
			else
				value = stmt.column_int (offset);
		}

		public int get () {
			if (! valid ()) {
				// throw
			}
			return value;
		}

		public bool valid () {
			return value >= 0;
		}

		public void bind_to_stmt (Statement stmt, int index) {
			if (valid ()) stmt.bind_int (index, value);
		}

	}
}