using Sqlite;
namespace Sqlp {
	public class Model : Record <ModelTable> {

		public string[] tags {
			
		}

		public string abbreviation;
		public string name;
		public string make;
		public string type;
		public bool total;

		public override int bind_for_save (Statement stmt) {
			var i = 1;
			stmt.bind_nonempty_text (i++, abbreviation);
			stmt.bind_nonempty_text (i++, name);
			stmt.bind_nonempty_text (i++, make);
			stmt.bind_nonempty_text (i++, type);
			stmt.bind_int (i++, (int) total);
			return i;
		}

		public override void set_from_stmt (Statement stmt) {
			var i = 1;
			abbreviation		= stmt.column_text (i++);
			name				= stmt.column_text (i++);
			make				= stmt.column_text (i++);
			type				= stmt.column_text (i++);
			total				= (bool) stmt.column_int (i++);
		}

	}
}