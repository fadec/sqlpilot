using GLib;

namespace SqlPilot {
	public struct Timezone {
		public string name;

		public Timezone (string tzname) {
			name = tzname;
		}

		public string to_iso8601 (Date date, TimeOfDay time) {
			var offset = utc_offset (date, time);
			if (offset == 0) return "Z";
			return "";
		}

		public string to_string () {
			return name;
		}

		public ulong utc_offset (Date date, TimeOfDay time) {
			
			return 0;
		}

		public void use () {
			if (! Environment.set_variable("TZ", (name == null) ? "UTC" : name, true)) {
				message ("Can not set timezone");
				return;
			}
			tzset();
		}

		public bool is_equal (Timezone t) {
			return t.name == name;
		}
	}

}