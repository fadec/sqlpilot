using GLib;
using Sqlite;

namespace Sqlp {
	public class Tag : Record {
		public string abbreviation { get; set; default = ""; }
		public string description { get; set; default = ""; }

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

		public override bool valid () {
			return (abbreviation.length > 0);
		}

	}
}