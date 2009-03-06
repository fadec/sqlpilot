using Sqlp;

public static int main ( string[] args ) {
	var date = Sqlp.Date.from_iso8601 ("1979-05-25");
	assert (date.to_iso8601 () == "1979-05-25");
	date.clear ();
	assert (! date.valid ());
	return 0;
}