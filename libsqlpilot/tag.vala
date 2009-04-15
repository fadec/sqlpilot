using GLib;
using Sqlite;

namespace Sqlp {
	public class Tag : Record <TagTable> {
		public string name { get; set; }
		public string abbreviation { get; set; }

		protected override void set_from_stmt (Statement stmt) {
			var i = 1;
			name = stmt.column_text (i++);
			abbreviation = stmt.column_text (i++);
		}
		protected override int bind_for_save (Statement stmt) {
			var i = 1;
			stmt.bind_text (i++, name);
			stmt.bind_text (i++, abbreviation);
			return i;
		}
	}
}