using Sqlite;

namespace Sqlp {
	public class AircraftCrud : Crud {

		Statement find_by_fleetno_stmt;
		public AircraftCrud ( Logbook logbook ) {
			base (logbook, "Aircraft");
			var find_by_fleetno_sql = "SELECT * FROM AIRCRAFT WHERE FleetNo = ?;";
			find_by_fleetno_stmt = logbook.prepare_statement (find_by_fleetno_sql);
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
		
		public Aircraft? find_by_fleetno (string fleetno) {
			find_by_fleetno_stmt.bind_text (1, fleetno);
			return find_first (find_by_fleetno_stmt);
		}

	}
}