using Sqlite;
namespace SqlPilot {
	public class Airport : Record {

		public string icao;
		public string iata;
		public string name;
		public string city;
		public string province;
		public string country;
		public string lat; //float
		public string lon; //float
		public string elev; //int
		public string tzone;
		public string notes;

		public Airport (AirportCrud crud) {
			base (crud);
		}

		public override int bind_for_save (Statement stmt) {
			var i = 1;
			bind_nonempty_text_else_null (stmt, i++, icao);
			bind_nonempty_text_else_null (stmt, i++, iata);
			bind_nonempty_text_else_null (stmt, i++, name);
			bind_nonempty_text_else_null (stmt, i++, city);
			bind_nonempty_text_else_null (stmt, i++, province);
			bind_nonempty_text_else_null (stmt, i++, country);
			bind_nonempty_text_else_null (stmt, i++, lat);
			bind_nonempty_text_else_null (stmt, i++, lon);
			bind_nonempty_text_else_null (stmt, i++, elev);
			i++; i++; // skip tzone offsets
			bind_nonempty_text_else_null (stmt, i++, tzone);
			bind_nonempty_text_else_null (stmt, i++, notes);
			return i;
		}

		public override void set_from_stmt (Statement stmt) {
			var i = 1;
			icao = stmt.column_text (i++);
			iata = stmt.column_text (i++);
			name = stmt.column_text (i++);
			city = stmt.column_text (i++);
			province = stmt.column_text (i++);
			country = stmt.column_text (i++);
			lat = stmt.column_text (i++);
			lon = stmt.column_text (i++);
			elev = stmt.column_text (i++);
			i++; i++;
			tzone = stmt.column_text (i++);
			notes = stmt.column_text (i++);
		}


	}

}