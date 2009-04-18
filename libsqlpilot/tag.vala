using GLib;
using Sqlite;

namespace Sqlp {
	public class Tag : Record <TagTable> {
		public string abbreviation { get; set; }
		public string description { get; set; }

		protected override void set_from_stmt (Statement stmt) {
			var i = 1;
			abbreviation = stmt.column_text (i++);
			description = stmt.column_text (i++);
		}
		protected override int bind_for_save (Statement stmt) {
			var i = 1;
			stmt.bind_text (i++, abbreviation);
			stmt.bind_text (i++, description);
			return i;
		}
	}
}