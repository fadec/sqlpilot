
using CSV;
public static int main ( string[] args ) {

	var reader = new Reader ( 10, 50 );

	try {
		reader.file ( "test.csv" );
	} catch {
//		stderr.printf ( "error - exiting\n" );
		return 1;
	}
	weak string[] fields;
	while ((fields = reader.read_row ()) != null) {
		foreach (string s in fields ) {
//			stderr.printf ("%s\n", s);
		}
	}

//	stderr.printf("Leaving Main\n");
	return 0;
}
