using Sqlite;

namespace SqlPilot {
	public class Aircraft : Record {
		public Model model;
		public string tail;
		public string fleetno;
		public string notes;

		public Aircraft (AircraftCrud crud) {
			base (crud);
		}

		public override int bind_for_save (Statement stmt) {
			var i = 1;
			// bind here and return id column
			return i;
		}

		public override void set_from_stmt (Statement stmt) {
			var i = 0;
		}

	}

}