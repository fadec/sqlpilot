
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
			var dur = Duration (69);
			return dur;
		}

		public int64 to_seconds () {
			return _seconds;
		}
	}
}