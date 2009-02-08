using GLib;

namespace SqlPilot {
	public struct Datetime {
		public Date date;
		public TimeOfDay time_of_day;

		public Datetime (Date d, TimeOfDay t) {
			date = d;
			time_of_day = t;
		}
		
		public bool valid () {
			return date.valid () && time_of_day.valid (); // but not all times are valid on all days. could round trip to mktime?
		}
//		public static Datetime from_iso8601 (string str) {
// 			string d;
// 			string t;
// 		    string format = "%as[^ T]%as";
// 			format.scanf (str, out d, out t);
// 			return Datetime (Date.from_iso8601 (d), TimeOfDay.from_iso8601 (t));
//		}

		// 2008-12-25T10:00-05:00
		// 2008-12-25T10:00+02:30
		public string to_iso8601 () {
			return date.to_iso8601 () + " " + time_of_day.to_iso8601 ();
		}

		public time_t mktime()
		{
			time_of_day.timezone.use ();
			return to_glib_time ().mktime ();
		}

		public time_t diff (Datetime other) {
			// return difftime (mktime (), other.mktime ());
			return mktime () - other.mktime ();
		}

		public Duration duration (Datetime other) {
			return Duration.from_seconds (diff (other));
		}

		
		public Datetime next_datetime_for_time_of_day (TimeOfDay t1) {
			var d1 = this.date;
			d1.subtract_days (1);
			Datetime dt1;
			time_t difference;
			do {
				dt1 = Datetime (d1, t1);
				difference = dt1.diff (this); // this call will cause tz changes, could be slow in loop
				d1.add_days (1);
			} while (difference < 0);
			return dt1;
		}

		public Duration duration_to_time_of_day (TimeOfDay t1) {
			return Duration.from_seconds (next_datetime_for_time_of_day (t1).diff (this));
		}

		// sets time and possibly changes date
		public void move_to_timezone (Timezone timezone) {
			if (timezone.is_equal (time_of_day.timezone)) return;
			var this_t = mktime ();
			timezone.use ();
			var tm = Time.local (this_t);
			time_of_day.timezone = timezone;
			time_of_day.set_hms (tm.hour, tm.minute, tm.second);
			return;
		}

		// same as move_to_timezone but returns a new struct instead of mutating this
		public Datetime in_timezone (Timezone timezone) {
			var new_datetime = this;
			new_datetime.move_to_timezone (timezone);
			return new_datetime;
		}

		private GLib.Time to_glib_time () {
			GLib.Time tm = {0};
			
			tm.year = date.get_year () - 1900;
			tm.month = date.get_month () - 1;
			tm.day = date.get_day ();
			tm.hour = time_of_day.get_hour ();
			tm.minute = time_of_day.get_minute ();
			tm.second = time_of_day.get_second ();
			tm.isdst = -1;
			return tm;
		}

	}
}