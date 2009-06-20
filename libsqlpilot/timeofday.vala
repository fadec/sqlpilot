using Sqlite;

namespace Sqlp {

	public struct TimeOfDay {

		public long seconds;
		public Timezone timezone;

		public int get_hour () { return (int)seconds / 3600; }
		public int get_minute () { return (int)(seconds % 3600) / 60; }
		public int get_second () { return (int)(seconds % 60); }

		public TimeOfDay () {
			seconds = -1;
			timezone = Timezone ("UTC");
		}

		public static TimeOfDay from_tzname_time (string timezone_string, string time_string) {
			assert (time_string != null);
			var tod = TimeOfDay ();
			if (time_string == "") {
				tod.seconds = -1;
			} else {
				tod.timezone = Timezone (timezone_string);
				tod.read_iso8601 (time_string);
			}
			return tod;
		}

		public static TimeOfDay from_timezone_time (Timezone timezone, string time_string) {
			assert (time_string != null);
			var tod = TimeOfDay ();
			if (time_string == "") {
				tod.seconds = -1;
			} else {
				tod.timezone = timezone;
				tod.read_iso8601 (time_string);
			}
			return tod;
		}

		public static TimeOfDay from_iso8601 (string time_string) {
			assert (time_string != null);
			var tod = TimeOfDay ();
			if (time_string == "") {
				tod.seconds = -1;
			} else {
				tod.timezone = Timezone ("UTC");
				tod.read_iso8601 (time_string);
			}
			return tod;
		}

		public bool valid () {
			return (seconds >= 0);
		}

		public string to_iso8601 () {
			var r = seconds;
			var h = r / 3600;
			var m = (r % 3600) / 60;
			//var s = r % 60;
			return (valid ()) ? "%02d:%02d".printf((int)h, (int)m) : "";
		}

		public void read_iso8601 (string time) {
			if (! time.validate ()) return;
			int h = 0;
			int m = 0;
			int s = 0;
			weak string rem = time;
			var digits = new StringBuilder ();
			do {
				if (rem[0].isdigit ()) {
					digits.append_unichar(rem[0]);
				}
			} while ((rem = rem.next_char ()) != "\0" && digits.len < 6);
			digits.str.scanf ("%2d%2d%2d", out h, out m, out s);
			set_hms (h, m, s);
			return;
		}

		public void set_hms (int h, int m, int s) {
			seconds = h * 3600 + m * 60 + s;
		}

		public TimeOfDay.from_stmt (Statement stmt, int offset) {
			if (stmt.column_type (offset) == Sqlite.NULL) {
				seconds = -1;
			} else {
				from_tzname_time ("UTC", stmt.column_text (offset));
			}
		}

// 		public void bind_to_stmt (Statement stmt, int index) {
// 			if (valid ()) {
// 				stmt.bind_text (index, to_iso8601 ());
// 			} else {
// 				stmt.bind_null (index);
// 			}
// 		}

	}
}