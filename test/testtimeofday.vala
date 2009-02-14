using Sqlp;

int main (string[] args) {
	var tod = TimeOfDay.from_tzname_time ("UTC", "");
	assert (! tod.valid ());
	assert (tod.to_iso8601 () == "");
	return 0;
}