using SqlPilot;

public static int main ( string[] args ) {
	var date = SqlPilot.Date ().from_iso8601 ("1979-01-15");
	var time = SqlPilot.TimeOfDay ().from_iso8601 ("10:00");
	var datetime = Datetime (date, time);
	assert (datetime.time_of_day.timezone.name == "UTC");
	assert (datetime.to_iso8601 () == "1979-01-15 10:00");
	datetime.move_to_timezone(Timezone ("US/Eastern"));
	assert (datetime.time_of_day.timezone.name == "US/Eastern");
	assert (datetime.to_iso8601 () == "1979-01-15 05:00");
	return 0;
}
