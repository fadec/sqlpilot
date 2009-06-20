namespace Sqlp {

	public static int main ( string[] args ) {
		var date = Sqlp.Date.from_iso8601 ("1979-01-15");
		var time = TimeOfDay.from_tzname_time ("UTC", "10:00");
		var datetime = Datetime (date, time);
		assert (datetime.time_of_day.timezone.name == "UTC");
		assert (datetime.to_iso8601 () == "1979-01-15 10:00");

		datetime.move_to_timezone (Timezone ("US/Eastern"));
		assert (datetime.time_of_day.timezone.name == "US/Eastern");
		assert (datetime.to_iso8601 () == "1979-01-15 05:00");

		datetime.time_of_day.timezone = Timezone ("US/Central");
		assert (datetime.to_iso8601 () == "1979-01-15 05:00");

		var dt1 = Datetime (Date ().from_iso8601 ("2009-07-04"), TimeOfDay.from_iso8601 ("21:00"));
		var dt2 = Datetime (Date ().from_iso8601 ("2009-07-04"), TimeOfDay.from_iso8601 ("22:00"));
		assert (dt2.diff (dt1) == (time_t)3600);

		dt1 = Datetime (Date ().from_iso8601 ("2009-07-04"), TimeOfDay.from_tzname_time ("US/Eastern", "22:00"));
		dt2 = Datetime (Date ().from_iso8601 ("2009-07-04"), TimeOfDay.from_timezone_time (Timezone ("US/Central"), "22:00"));

		assert (dt2.diff (dt1) == (time_t) 3600);
		assert (dt2.duration (dt1).to_minutes () == 60);

// 		dt1 = Datetime.from_iso8601 ("2009-01-01T12:30");
// 		message (dt1.to_iso8601 ());
// 		dt2 = Datetime (Date ().from_iso8601 ("2009-01-01"), TimeOfDay.from_iso8601 ("22:00"));
// 		assert (dt2.diff (dt1) == 0);

		// duration from time of day
		dt1 = Datetime (Date ().from_iso8601 ("2009-07-04"), TimeOfDay.from_iso8601 ("09:00"));
		time = TimeOfDay.from_iso8601 ("09:30");
		dt2 = dt1.next_datetime_for_time_of_day (time);
		assert (dt2.to_iso8601 () == "2009-07-04 09:30");
		assert ((int)(dt1.duration_to_time_of_day (time).to_minutes ()) == 30);
		time = TimeOfDay.from_iso8601 ("08:00");
		assert (dt1.duration_to_time_of_day (time).to_hours () == 23);


		dt1 = Datetime (Date ().from_iso8601 ("2009-07-04"), TimeOfDay.from_iso8601 ("09:00"));
		var dur = Duration.from_minutes (85);
		dt2 = dt1.add (dur);
		assert (dt2.diff (dt1) == 85 * 60);

		dt1 = Datetime (Date ().from_iso8601 ("2009-07-04"), TimeOfDay.from_iso8601 ("09:00"));
		dur = Duration.from_minutes (85);
		dt2 = dt1.subtract (dur);
		assert (dt2.diff (dt1) == -85 * 60);

		return 0;
	}
}