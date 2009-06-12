using GLib;

namespace Sqlp {
	public struct Date {
		public GLib.Date gdate;

		private	const string[] month_names =
			{ "Invalid"	,
			  "January"	, "Febuary"  , "March",
			  "April"	, "May"      , "June",
			  "July"	, "August"   , "September",
			  "October"	, "November" , "December" };

		private	const string[] month_abbreviations =
			{ "Inv"	,
			  "Jan"	, "Feb"  , "Mar",
			  "Apr"	, "May"  , "Jun",
			  "Jul"	, "Aug"  , "Sep",
			  "Oct"	, "Nov"  , "Dec" };		

		public int get_year () { return (int) gdate.get_year (); }
		public int get_month () { return (int) gdate.get_month (); }
		public int get_day () { return (int) gdate.get_day (); }

		public static Date from_iso8601 (string date_string) {
			var date = Date ();
			date.read_iso8601 (date_string);
			return date;
		}

		public string to_iso8601 () {
			if (gdate.valid ()) {
				var s = "0000-00-00";			
				gdate.strftime((char[])s, "%Y-%m-%d");
				return s;
			} else {
				// perhaps should throw
				return "";
			}
		}

		public void read_iso8601 (string datestring) {
			GLib.DateDay d;
			int m;
			GLib.DateYear y;
			datestring.scanf("%d-%d-%d", out y, out m, out d);
 			if (GLib.Date.valid_dmy (d, (GLib.DateMonth)m, y)) gdate.set_dmy (d, m, y);
		}
		
		public void add_days (uint days) {
			// perhaps should throw if invalid
			gdate.add_days (days);
		}
		
		public void subtract_days (uint days) {
			gdate.subtract_days (days);
		}
		
		public bool valid () {
			return gdate.valid ();
		}
		
		public void clear () {
			gdate.clear ();
		}

		public void set_dmy (DateDay d, DateMonth m, DateYear y) {
			gdate.set_dmy (d, m, y);
		}

 		public static string month_name (int m) {
 			return month_names[(m >= 1 && m <= 12) ? m : 0];
 		}

 		public static string month_abbreviation (int m) {
 			return month_abbreviations[(m >= 1 && m <= 12) ? m : 0];
 		}
	}
}