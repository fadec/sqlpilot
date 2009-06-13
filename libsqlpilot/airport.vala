using Sqlite;

namespace Sqlp {
	public class Airport : Record <AirportTable> {

		public enum KeyType {
			ICAO, IATA, USER
		}

		public struct KeyPreference {
			public KeyType p1;
			public KeyType p2;
			public KeyType p3;

			public KeyPreference (KeyType a, KeyType b, KeyType c) {
				p1 = a;
				p2 = b;
				p3 = c;
			}
		}

		public string icao { get; set; default = ""; }
		public string iata { get; set; default = ""; }
		public string abbreviation { get; set; default = ""; }
		public string name { get; set; default = ""; }
		public string city { get; set; default = ""; }
		public string province { get; set; default = ""; }
		public string country { get; set; default = ""; }
		public string lat { get; set; default = ""; } //float
		public string lon { get; set; default = ""; } //float
		public string elev { get; set; default = ""; } //int
		public Timezone timezone = Timezone ("UTC");
		public string notes { get; set; default = ""; }

		public override int bind_for_save (Statement stmt) {
			var i = 1;
			stmt.bind_nonempty_text (i++, icao);
			stmt.bind_nonempty_text (i++, iata);
			stmt.bind_nonempty_text (i++, abbreviation);
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
			abbreviation = stmt.column_text (i++);
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
			case 3: iata = ident;
				break;
			case 4: icao = ident;
				break;
			case 5: // reserved for fixes
				break;
			default: abbreviation = ident;
				break;
			}
		}

		public string show (KeyPreference kp) {
			switch (kp.p1) {
			case KeyType.ICAO: if (icao.length > 0) return icao; break;
			case KeyType.IATA: if (iata.length > 0) return iata; break;
			case KeyType.USER: if (abbreviation.length > 0) return abbreviation; break;
			}
			switch (kp.p2) {
			case KeyType.ICAO: if (icao.length > 0) return icao; break;
			case KeyType.IATA: if (iata.length > 0) return iata; break;
			case KeyType.USER: if (abbreviation.length > 0) return abbreviation; break;
			}
			switch (kp.p3) {
			case KeyType.ICAO: if (icao.length > 0) return icao; break;
			case KeyType.IATA: if (iata.length > 0) return iata; break;
			case KeyType.USER: if (abbreviation.length > 0) return abbreviation; break;
			}
			return "";
		}

		public override bool valid () {
			if (icao.length != 0 && icao.length != 4) return false;
			if (iata.length != 0 && iata.length != 3) return false;
			if (abbreviation.length != 0 &&
				(abbreviation.length == 1 ||
				 abbreviation.length == 3 ||
				 abbreviation.length == 4 ||
				 abbreviation.length == 5)) return false;
			if ((abbreviation.length == 0) &&
				(icao.length == 0) &&
				(iata.length == 0)) return false;
			return true;
		}
	}

}