using GLib;

namespace SqlPilot {
	public class Date {
		private GLib.Date gdate;

		public Date () {
		}

		public Date set_from_iso8601 ( string date ) {
			return this;
		}

		public string to_iso8601 () {
			return "not implemented";
		}

	}

}