// valac test.vala libcsv.vapi --Xcc=-I.. --Xcc=.libs/csv.o --Xcc=.libs/ccsv.o -o test --save-temps && ./test
using CSV;
public static int main ( string[] args ) {

	var reader = new Reader ( 10, 50 );

	try {
		reader.open ( "test.csv" );
	} catch {
		stderr.printf ( "error - exiting\n" );
		return 1;
	}
	weak string[] fields;
	while ((fields = reader.read_row ()) != null) {
		foreach (string s in fields ) {
			stdout.printf ("%s\n", s);
		}
	}

	stderr.printf("Leaving Main\n");
	return 0;
}
