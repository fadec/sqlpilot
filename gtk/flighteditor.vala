using Sqlite;
using Gtk;
namespace SqlpGtk {
	public class FlightEditor : Editor {

		private weak Book book;
		
		public FlightEditor ( Book book ) {
			base (new Pane ("airline_flight_fields"), new Pane ("flight_toolbar"));
			this.book = book;
		}

		private override Statement? prepare_list_statement () {
			var where_clause = "1 = 1";
			var stmt = book.logbook.prepare_statement (
				"SELECT * FROM Flights WHERE " + where_clause + ";");
			return stmt;
		}
	}
}