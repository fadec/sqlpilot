using Sqlite;
namespace SqlPilot {
	public class Airport : Record {

		public string icao;
		public string iata;
		public string name;
		public string city;
		public string province;
		public string country;
		public float  lat;
		public float  lon;
		public int    elev;
		public float  off_utc;
		public float  off_dst;
		public string tzone;
		public string notes;

		public Airport (AirportCrud crud) {
			base (crud);
		}

		public override int bind_for_save (Statement stmt) {
			var i = 1;
			// bind here and return id column
			return i;
		}

		public override void set_from_stmt (Statement stmt) {
			var i = 0;
		}


	}

}