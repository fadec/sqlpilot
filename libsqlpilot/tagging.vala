using GLib;
using Sqlite;

namespace Sqlp {
	public class Tagging : Record, IJoinRecord {

		public int64 parent_id { get; set; }
		public int64 child_id { get; set; }

		protected override void set_from_stmt (Statement stmt) {
			var i = 1;
			parent_id = stmt.column_int64 (i++);
			child_id = stmt.column_int64 (i++);
		}

		protected override int bind_for_save (Statement stmt) {
			var i = 1;
			stmt.bind_nonzero_int64 (i++, parent_id);
			stmt.bind_nonzero_int64 (i++, child_id);
			return i;
		}
	}
}
