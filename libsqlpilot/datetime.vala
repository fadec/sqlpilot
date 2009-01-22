using GLib;

namespace SqlPilot {
	public struct Datetime {
		public Date date;
		public TimeOfDay time_of_day;

		public Datetime (Date d, TimeOfDay t) {
			date = d;
			time_of_day = t;
		}

		// 2008-12-25T10:00-05:00
		// 2008-12-25T10:00+02:30
		public string to_iso8601 () {
			return date.to_iso8601 () + " " + time_of_day.to_iso8601 ();
		}

		time_t mktime()
		{
			time_of_day.timezone.use ();
			return to_glib_time ().mktime ();
		}

		// sets time and possibly changes date
		public Datetime move_to_timezone (Timezone timezone) {
			if (timezone.is_equal (time_of_day.timezone)) return this;
			var this_t = mktime ();
			timezone.use ();
			var tm = Time.local (this_t);
			time_of_day.timezone = timezone;
			time_of_day.set_hms (tm.hour, tm.minute, tm.second);
			return this;
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