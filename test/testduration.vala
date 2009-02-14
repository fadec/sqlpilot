using Sqlp;

static int main ( string[] args ) {
	var d1 = Duration.from_string ("1+32");
	var d2 = Duration.from_minutes (92);
	assert (d1.to_seconds () == d2.to_seconds ());
	assert (Duration.from_string ("0+32").to_minutes () == 32);
	assert (Duration.from_string ("100+01").to_minutes () == 6001);
	return 0;
}