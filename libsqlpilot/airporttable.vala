using Sqlite;
namespace Sqlp {
	public class AirportTable : Table <Logbook, Airport> {

		private Statement find_by_ident_stmt;

		public AirportTable ( Logbook logbook ) {
			this.record_type = typeof (Airport);
			this.database = logbook;
			this.table_name = "Airports";
		}

		construct {
			var find_by_ident_sql = "SELECT * FROM Airports WHERE ICAO = ? OR IATA = ? OR Abbreviation = ?;";
			find_by_ident_stmt = database.prepare_statement (find_by_ident_sql);
		}

		public Airport? find_by_ident (string ident) {
			find_by_ident_stmt.bind_text (1, ident);
			find_by_ident_stmt.bind_text (2, ident);
			find_by_ident_stmt.bind_text (3, ident);
			return (find_first (find_by_ident_stmt) as Airport);
		}

		public Airport? find_or_create_by_ident (string ident) {
			var ap = find_by_ident (ident);
			if (ap == null) {
				ap = new_record ();
				ap.set_ident (ident);
			}
			return ap.save () ? ap : null;
		}
	}
}