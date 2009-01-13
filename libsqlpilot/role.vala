using Sqlite;
namespace SqlPilot {
	public class Role : Record {
		public string ident = "";
		public string name = "";
		public bool pic;
		public bool sic;
		public bool fe;
		public bool solo;
		public bool dual;
		public bool instruct;
		public bool total;

		public Role ( RoleCrud crud ) {
			base (crud);
		}

		public override string to_string () {
			return "<Role %s>".printf (ident);
		}

		protected override int bind_for_save (Statement stmt) {
			var i = 1;
			bind_nonempty_text_else_null (stmt, i++, ident);
			bind_nonempty_text_else_null (stmt, i++, name);
			stmt.bind_int (i++, (int) pic);
			stmt.bind_int (i++, (int) sic);
			stmt.bind_int (i++, (int) fe);
			stmt.bind_int (i++, (int) solo);
			stmt.bind_int (i++, (int) dual);
			stmt.bind_int (i++, (int) instruct);
			stmt.bind_int (i++, (int) total);
			return i;
			
		}

		protected override void set_from_stmt (Statement stmt) {
			var i = 1;
			ident = stmt.column_text(i++);
			name = stmt.column_text(i++);
			pic = (bool) stmt.column_int(i++);
			sic = (bool) stmt.column_int(i++);
			fe = (bool) stmt.column_int(i++);
			solo = (bool) stmt.column_int(i++);
			dual = (bool) stmt.column_int(i++);
			instruct = (bool) stmt.column_int(i++);
			total = (bool) stmt.column_int(i++);
		}

	}
}