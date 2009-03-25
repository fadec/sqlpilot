using Sqlite;
namespace Sqlp {
	public class FlightTable : Table <Logbook, Flight> {

		protected Statement find_by_date_fltno_dep_arr_stmt;
		protected Statement find_by_role_id_stmt;

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
			"(destination.ICAO = ? OR destination.IATA = ?);";
			find_by_date_fltno_dep_arr_stmt = database.prepare_statement (
				find_by_date_fltno_dep_arr_sql);
			find_by_role_id_stmt = database.prepare_statement (
				"SELECT * FROM Flights WHERE role_id = ?;");
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
			return find_first (stmt);
		}

		public List <Flight> find_by_role_id (int64 role_id) {
			weak Statement stmt = find_by_role_id_stmt;
			stmt.bind_int64 (1, role_id);
			return find_all (stmt);
		}
	}
}