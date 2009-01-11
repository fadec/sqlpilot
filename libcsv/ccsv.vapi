// Interface to C csv library
[CCode (lower_case_cprefix ="csv_", upper_case_cprefix="CSV_", cheader_filename="ccsv.h")]
namespace CSV {

	public static int row_fread(GLib.FileStream in,
									uchar* buf,
									size_t bn,
									uchar** row,
									int numcols,
									int sep,
									int flags);

	public static int row_parse_str(string src,
									size_t sn,
									uchar* buf,
									size_t bn,
									uchar** row,
									int numcols,
									char sep,
									int flags);
}