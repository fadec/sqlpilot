using Sqlite;

namespace SqlPilot {
	public class AircraftCrud : Crud {

		public AircraftCrud ( Logbook logbook ) {
			base (logbook, "Aircraft");
		}

		public override Record new_record () {
			return new Aircraft ( this ) as Record;
		}
		public Aircraft beget () { return new_record () as Aircraft; }

		public Aircraft? find_first (Statement stmt) {
			return (record_find_first (stmt) as Aircraft);
		}
		
 		public Aircraft? find_by_id (int64 id) {
			return (record_find_by_id (id) as Aircraft);
 		}


	}
}