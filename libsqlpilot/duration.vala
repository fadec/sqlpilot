
namespace Sqlp {

	public struct Duration {
		private long _seconds;
		private bool _valid;

		public Duration (long seconds, bool valid = true) {
			_seconds = seconds;
			_valid = valid;
		}

		public static Duration from_seconds ( long seconds ) {
			return Duration (seconds);
		}
		public static Duration from_minutes ( long minutes ) {
			return Duration (minutes * 60);
		}

		public static Duration from_string ( string str ) {
			if ((str == null) || (! str.validate ()) || str[0] == '\0') return Duration (0, false);
			int a = 0;
			int b = 0;
			var matched = str.scanf ("%d+%d", out a, out b);
			return from_minutes ((matched - 1) * 60 * a + b);
		}

		public time_t to_time_t () {
			return (time_t) _seconds;
		}

		public long to_seconds () {
			return _seconds;
		}

		public long to_minutes () {
			return _seconds / 60;
		}

		public long to_hours () {
			return _seconds / 3600;
		}

		public string to_string () {
			var s = "%d+%d".printf ((int)(_seconds / 3600), (int)((_seconds / 60) % 60));
			message (s);
			return s;
		}

		public bool valid () {
			return _valid;
		}
	}
}