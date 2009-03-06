using Sqlite;
namespace Sqlp {
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
		public Timezone timezone = Timezone ("UTC");
		public string notes;

		public override int bind_for_save (Statement stmt) {
			var i = 1;
			stmt.bind_nonempty_text (i++, icao);
			stmt.bind_nonempty_text (i++, iata);
			stmt.bind_nonempty_text (i++, name);
			stmt.bind_nonempty_text (i++, city);
			stmt.bind_nonempty_text (i++, province);
			stmt.bind_nonempty_text (i++, country);
			stmt.bind_nonempty_text (i++, lat);
			stmt.bind_nonempty_text (i++, lon);
			stmt.bind_nonempty_text (i++, elev);
			i++; i++; // skip timezone offsets
			stmt.bind_nonempty_text (i++, timezone.name);
			stmt.bind_nonempty_text (i++, notes);
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
			timezone = Timezone (stmt.column_text (i++));
			notes = stmt.column_text (i++);
		}

		public void set_ident (string? ident) {
			if (ident == null) return;
			switch (ident.length) {
			case 3: iata = ident; break;
			case 4: icao = ident; break;
			}
		}
	}

}