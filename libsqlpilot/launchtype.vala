using Sqlite;
namespace Sqlp {
	public class LaunchType : Record {

		public string abbreviation { get; set; }
		public string description { get; set; }

		public override int bind_for_save (Statement stmt) {
			var i = 1;
			stmt.bind_nonempty_text (i++, abbreviation);
			stmt.bind_nonempty_text (i++, description);
			return i;
		}

		public override void set_from_stmt (Statement stmt) {
			var i = 1;
			abbreviation = stmt.column_text (i++);
			description = stmt.column_text (i++);
		}

	}

}