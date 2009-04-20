using Sqlite;
namespace Sqlp {
	public class Surface : Record {
		public string abbreviation { get; set; default = ""; }
		public string description { get; set; default = ""; }

		private List <Takeoff> _takeoffs;
		public List <Takeoff> takeoffs {
			get {
				if (_takeoffs == null) {
					_takeoffs = (table.database as Logbook).takeoffs.find_by_surface_id (id);
				}
				return _takeoffs;
			}
		}

		private List <Landing> _landings;
		public List <Landing> landings {
			get {
				if (_landings == null) {
					_landings = (table.database as Logbook).landings.find_by_surface_id (id);
				}
				return _landings;
			}
		}

		public override string summary () {
			return "<Surface %s>".printf (abbreviation);
		}

		construct {
			message ("new surface made");
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
			return takeoffs.length () == 0 && landings.length () == 0;
		}

		protected override void before_save () {
			if (abbreviation == "") abbreviation = null;
			if (description == "") description = null;
		}

		protected override bool valid () {
			if (! is_unique_text((table as SurfaceTable).unique_abbreviation_stmt, abbreviation)) {
				return false;
			}
			return true;
		}
	}
}
