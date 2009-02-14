using Sqlite;
namespace Sqlp {
	public class AirportCrud : Crud {

		private Statement find_by_ident_stmt;

		public AirportCrud ( Logbook logbook ) {
			base (logbook, "Airports");
			var find_by_ident_sql = "SELECT * FROM Airports WHERE ICAO = ? OR IATA = ?;";
			find_by_ident_stmt = logbook.prepare_statement (find_by_ident_sql);
		}

		public override Record new_record () {
			return new Airport ( this ) as Record;
		}
		public Airport beget () { return new_record () as Airport; }

 		public Airport? find_by_id (int64 id) {
			return (record_find_by_id (id) as Airport);
 		}

		public Airport? find_by_ident (string ident) {
			find_by_ident_stmt.bind_text (1, ident);
			find_by_ident_stmt.bind_text (2, ident);
			return (record_find_first (find_by_ident_stmt) as Airport);
		}
	}
}