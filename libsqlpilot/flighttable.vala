using Sqlite;
namespace Sqlp {
	public class FlightTable : Table {

		protected Statement find_by_date_fltno_dep_arr_stmt;
		protected Statement find_by_role_id_stmt;
		protected Statement find_by_date_leg_stmt;

		public FlightTable ( Logbook logbook ) {
			this.record_type = typeof (Flight);
			this.database = logbook;
			this.table_name = "Flights";
		}

		construct {
			var find_by_date_fltno_dep_arr_sql =
			"SELECT * FROM Flights " +
			"LEFT JOIN Airports origin ON origin_airport_id = origin.id " +
			"LEFT JOIN Airports destination ON destination_airport_id = destination.id " +
			"WHERE Date = ? AND FlightNumber = ? AND " +
			"(origin.ICAO = ? OR destination.IATA = ?) AND " +
			"(destination.ICAO = ? OR destination.IATA = ?)";
			find_by_date_fltno_dep_arr_stmt = database.prepare_statement (
				find_by_date_fltno_dep_arr_sql);
			
			find_by_role_id_stmt = database.prepare_statement (
				"SELECT * FROM Flights WHERE role_id = ?");
			
			find_by_date_leg_stmt = database.prepare_statement (
				"SELECT * FROM Flights WHERE Date = ? AND Leg = ?");
		}

		public Flight? find_by_date_fltno_dep_arr (string date_str,
												   string fltno,
												   string dep_ident,
												   string arr_ident) {
			weak Statement stmt = find_by_date_fltno_dep_arr_stmt;
			var i = 1;
			stmt.bind_text (i++, date_str);
			stmt.bind_text (i++, fltno);
			stmt.bind_text (i++, dep_ident);
			stmt.bind_text (i++, dep_ident);
			stmt.bind_text (i++, arr_ident);
			stmt.bind_text (i++, arr_ident);
			return find_first (stmt) as Flight?;
		}

		public List <Flight> find_by_role_id (int64 role_id) {
			weak Statement stmt = find_by_role_id_stmt;
			stmt.bind_nonzero_int64 (1, role_id);
			return find_all (stmt);
		}
		
		public Flight? find_by_date_leg (Date date, int leg) {
			weak Statement stmt = find_by_date_leg_stmt;
			stmt.bind_text (1, date.to_iso8601 ());
			stmt.bind_int (2, leg);
			return find_first (stmt) as Flight?;
		}
	}
}