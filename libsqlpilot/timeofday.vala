namespace SqlPilot {

	public struct TimeOfDay {

		public ulong seconds;
		public Timezone timezone;

		public int get_hour () { return (int)seconds / 3600; }
		public int get_minute () { return (int)(seconds % 3600) / 60; }
		public int get_second () { return (int)(seconds % 60); }

		public TimeOfDay () {
			seconds = 0;
  			this.timezone = Timezone ("UTC");
		}

		public string to_iso8601 () {
			var r = seconds;
			var h = r / 3600;
			var m = (r % 3600) / 60;
			//var s = r % 60;
			return "%02d:%02d".printf((int)h, (int)m);
		}

// Confuses vala with circular dependency
// 		public Datetime to_datetime (Date date) {
// 			var dt = Datetime ();
// 			dt.date = date;
// 			dt.time_of_day = this;
// 			return dt;
// 		}

		public TimeOfDay from_iso8601 (string time) {
			if ((time == null) || (! time.validate ())) return this;
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
			return this;
		}

		public TimeOfDay set_hms (int h, int m, int s) {
			seconds = h * 3600 + m * 60 + s;
			return this;
		}

		public TimeOfDay set_timezone (Timezone tz) {
			this.timezone = tz;
			return this;
		}

	}
}