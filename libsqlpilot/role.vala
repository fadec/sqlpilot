using Sqlite;
namespace Sqlp {
	public class Role : Record {
		public string abbreviation = "";
		public string name = "";
		public bool pic;
		public bool sic;
		public bool fe;
		public bool solo;
		public bool dual;
		public bool instruct;
		public bool military;
		public bool total;

		public override string to_string () {
			return "<Role %s>".printf (abbreviation);
		}

		protected override int bind_for_save (Statement stmt) {
			var i = 1;
			stmt.bind_nonempty_text (i++, abbreviation);
			stmt.bind_nonempty_text (i++, name);
			stmt.bind_int (i++, (int) pic);
			stmt.bind_int (i++, (int) sic);
			stmt.bind_int (i++, (int) fe);
			stmt.bind_int (i++, (int) solo);
			stmt.bind_int (i++, (int) dual);
			stmt.bind_int (i++, (int) instruct);
			stmt.bind_int (i++, (int) military);
			stmt.bind_int (i++, (int) total);
			return i;
			
		}

		protected override void set_from_stmt (Statement stmt) {
			var i = 1;
			abbreviation = stmt.column_text(i++);
			name = stmt.column_text (i++);
			pic = (bool) stmt.column_int (i++);
			sic = (bool) stmt.column_int (i++);
			fe = (bool) stmt.column_int (i++);
			solo = (bool) stmt.column_int (i++);
			dual = (bool) stmt.column_int (i++);
			instruct = (bool) stmt.column_int (i++);
			military = (bool) stmt.column_int (i++);
			total = (bool) stmt.column_int (i++);
		}

	}
}