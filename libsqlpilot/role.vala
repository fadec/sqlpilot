using Sqlite;
namespace Sqlp {
	public class Role : Record <RoleTable> {
		public string abbreviation { get; set; default = ""; }
		public string name { get; set; default = ""; }
		public bool total { get; set; default = false; }
		public bool pic { get; set; default = false; }
		public bool sic { get; set; default = false; }
		public bool flight_engineer { get; set; default = false; }
		public bool solo { get; set; default = false; }
		public bool dual_received { get; set; default = false; }
		public bool instructor { get; set; default = false; }
		public bool military { get; set; default = false; }

		private List <Flight> _flights;
		public List <Flight> flights {
			get {
				if (_flights == null) {
					_flights = table.database.flight.find_by_role_id (id);
				}
				return _flights;
			}
		}

		public override string summary () {
			return "<Role %s>".printf (abbreviation);
		}

		construct {
			message ("new role made");
		}

		protected override int bind_for_save (Statement stmt) {
			var i = 1;
			stmt.bind_nonempty_text (i++, abbreviation);
			stmt.bind_nonempty_text (i++, name);
			stmt.bind_int (i++, (int) total);
			stmt.bind_int (i++, (int) pic);
			stmt.bind_int (i++, (int) sic);
			stmt.bind_int (i++, (int) flight_engineer);
			stmt.bind_int (i++, (int) solo);
			stmt.bind_int (i++, (int) dual_received);
			stmt.bind_int (i++, (int) instructor);
			stmt.bind_int (i++, (int) military);
			return i;
		}

		protected override void set_from_stmt (Statement stmt) {
			var i = 1;
			abbreviation = stmt.column_text(i++);
			name = stmt.column_text (i++);
			total = (bool) stmt.column_int (i++);
			pic = (bool) stmt.column_int (i++);
			sic = (bool) stmt.column_int (i++);
			flight_engineer = (bool) stmt.column_int (i++);
			solo = (bool) stmt.column_int (i++);
			dual_received = (bool) stmt.column_int (i++);
			instructor = (bool) stmt.column_int (i++);
			military = (bool) stmt.column_int (i++);
		}

		public override bool deletable () {
			return flights.length () == 0;
		}

		protected override void before_save () {
			if (abbreviation == "") abbreviation = null;
			if (name == "") name = null;
		}

		protected override bool valid () {
			if (! is_unique_text(table.unique_abbreviation_stmt, abbreviation)) {
				return false;
			}
			return true;
		}
	}
}
