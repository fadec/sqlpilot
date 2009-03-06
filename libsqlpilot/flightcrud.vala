using Sqlite;
namespace Sqlp {
	public class FlightCrud : Crud <Flight> {

		protected Statement find_by_date_fltno_dep_arr_stmt;

		public FlightCrud ( Logbook logbook ) {
			this.record_type = typeof (Flight);
			this.logbook = logbook;
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
			find_by_date_fltno_dep_arr_stmt =
			logbook.prepare_statement (find_by_date_fltno_dep_arr_sql);
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

	}
}