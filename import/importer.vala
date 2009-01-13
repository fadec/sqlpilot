using GLib;
using CSV;

namespace SqlPilot {
	public class Importer {
		private Logbook logbook;
		private HashTable<string, int> columns;

		public Importer ( Logbook logb ) {
			logbook = logb;
		}

		public void do_import ( string csvtext ) {
			read_column_headers ();
		}

		private void read_column_headers () {
			
		}	
	}

}

public static int main (string[] args) {
	return 0;
}