using Sqlite;
namespace SqlPilot {
	public class FlightCrud : Crud {

		protected Statement find_by_date_fltno_dep_arr_stmt;

		public FlightCrud ( Logbook logbook ) {
			base (logbook, "Flights");
			var find_by_date_fltno_dep_arr_sql =
			"SELECT * FROM Flights " +
			"LEFT JOIN Airports dep ON dep_id = dep.id " +
			"LEFT JOIN Airports arr ON arr_id = arr.id " +
			"WHERE Date = ? AND FltNo = ? AND " +
			"(dep.ICAO = ? OR dep.IATA = ?) AND " +
			"(arr.ICAO = ? OR arr.IATA = ?);";
			find_by_date_fltno_dep_arr_stmt =
			logbook.prepare_statement (find_by_date_fltno_dep_arr_sql);
		}

		public override Record new_record () {
			return new Flight ( this ) as Record;
		}
		public Flight beget () { return new_record () as Flight; }

 		public Flight? find_by_id (int64 id) {
			return (record_find_by_id (id) as Flight);
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
			if (stmt.step () == ROW) {
				var flight = beget ();
				flight.set_from_stmt (stmt);
				flight.is_modified = false;
				return flight;
			} else {
				return null;
			}

		}

	}
}