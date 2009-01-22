using SqlPilot;

public static int main ( string[] args ) {
	var date = SqlPilot.Date.from_iso8601 ("1979-05-25");
	assert (date.to_iso8601 () == "1979-05-25");
	return 0;
}