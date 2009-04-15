using GLib;
using Sqlite;

namespace Sqlp {
	public class Tagging : Record <TaggingTable> {

		public int64 object_id { get; set; }
		public int64 tag_id { get; set; }

		protected override void set_from_stmt (Statement stmt) {
			var i = 1;
			object_id = stmt.column_int64 (i++);
			tag_id = stmt.column_int64 (i++);
		}
		protected override int bind_for_save (Statement stmt) {
			var i = 1;
			stmt.bind_int64 (i++, object_id);
			stmt.bind_int64 (i++, tag_id);
			return i;
		}
	}
}
