using GLib;

namespace Sqlp {
	public struct Date {
		public GLib.Date gdate;

		public int get_year () { return (int) gdate.get_year (); }
		public int get_month () { return (int) gdate.get_month (); }
		public int get_day () { return (int) gdate.get_day (); }

		public static Date from_iso8601 (string date_string) {
			var date = Date ();
			date.read_iso8601 (date_string);
			return date;
		}

		public string to_iso8601 () {
			var s = "0000-00-00";
			if (gdate.valid ()) {
				gdate.strftime((char[])s, "%Y-%m-%d");
			}
			return s;
		}

		public void read_iso8601 (string date) {
			GLib.DateDay d;
			int m;
			GLib.DateYear y;
			date.scanf("%d-%d-%d", out y, out m, out d);
 			gdate.set_dmy(d,m,y);
		}
		
		public void add_days (uint days) {
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
	}
}