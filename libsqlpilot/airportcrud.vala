using Sqlite;
namespace Sqlp {
	public class AirportCrud : Crud <Airport> {

		private Statement find_by_ident_stmt;

		public AirportCrud ( Logbook logbook ) {
			this.record_type = typeof (Airport);
			this.logbook = logbook;
			this.table_name = "Airports";
		}

		construct {
			var find_by_ident_sql = "SELECT * FROM Airports WHERE ICAO = ? OR IATA = ?;";
			find_by_ident_stmt = logbook.prepare_statement (find_by_ident_sql);
		}

		public Airport? find_by_ident (string ident) {
			find_by_ident_stmt.bind_text (1, ident);
			find_by_ident_stmt.bind_text (2, ident);
			return (find_first (find_by_ident_stmt) as Airport);
		}
	}
}