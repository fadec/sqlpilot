using Sqlite;
namespace Sqlp {
	public class Role : Record <RoleTable> {
		public string abbreviation { get; set; default = ""; }
		public string description { get; set; default = ""; }

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
			stmt.bind_nonempty_text (i++, description);
			return i;
		}

		protected override void set_from_stmt (Statement stmt) {
			var i = 1;
			abbreviation = stmt.column_text(i++);
			description = stmt.column_text (i++);
		}

		public override bool deletable () {
			return flights.length () == 0;
		}

		protected override void before_save () {
			if (abbreviation == "") abbreviation = null;
			if (description == "") description = null;
		}

		protected override bool valid () {
			if (! is_unique_text(table.unique_abbreviation_stmt, abbreviation)) {
				return false;
			}
			return true;
		}
	}
}
