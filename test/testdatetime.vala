namespace SqlPilot {

	public static int main ( string[] args ) {
		var date = SqlPilot.Date.from_iso8601 ("1979-01-15");
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

		return 0;
	}
}