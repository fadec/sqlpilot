
namespace SqlPilot {

	public struct Duration {
		private int64 _seconds;
		
		public Duration (int64 seconds) {
			_seconds = seconds;
		}

		public static Duration from_seconds ( int64 seconds ) {
			return Duration (seconds);
		}
		public static Duration from_minutes ( int64 minutes ) {
			return Duration (minutes * 60);
		}

		public static Duration from_string ( string str ) {
			if ((str == null) || (! str.validate ())) return from_minutes (0);
			int a = 0;
			int b = 0;
			var matched = str.scanf ("%d+%d", out a, out b);
			return from_minutes ((matched - 1) * 60 * a + b);
		}

		public int64 to_seconds () {
			return _seconds;
		}

		public int64 to_minutes () {
			return _seconds / 60;
		}

		public int64 to_hours () {
			return _seconds / 3600;
		}

		public string to_string () {
			return "%l+%l".printf (_seconds / 3600, (_seconds / 60) % 60);
		}
	}
}