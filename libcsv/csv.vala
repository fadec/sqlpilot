using GLib;

namespace CSV {
	public errordomain CSVError { INVALID }

	public class Reader {

		public char sep;

		private FileStream instream;
		private char* buffer;
		private char** row;
		private weak string[] row_export;
		private int ncolumns;
		private int nbuffer;

		public Reader ( int ncol, int nbuf ) {
			instream = null;
			sep = ',';
			nbuffer = nbuf;
			ncolumns = ncol;
			buffer = malloc(nbuffer * sizeof(char));
			row = malloc(ncolumns * sizeof(char*));
		}

		public void open ( string filename ) throws FileError {
			// previos instream should be closed automatically on unref
			instream = FileStream.open ( filename, "r" );
		}

		public weak string[]? read_row () {
			if (instream != null &&
				row_fread (instream,
						   (uchar*) buffer,
						   nbuffer * sizeof(char),
						   (uchar**) row,
						   ncolumns,
						   sep,
						   0x02) > 0) {
				row_export = (string[]) row;
				row_export.length = ncolumns;
				return row_export;
			} else return null;
		}

		~Reader () {
 			free (row);
 			free (buffer);
		}
	}
}