namespace SqlPilot {

	public class TimeOfDay {
		private ulong _seconds;

		public string to_iso8601 () {
			return "not implemented";
		}

		public TimeOfDay to_utc () {
			return this;
		}

		public TimeOfDay set_from_iso8601 ( string time ) {
			return this;
		}

	}

}