
namespace SqlPilot {

	public struct Duration {
		private int64 _seconds;
		
		public int64 to_seconds () {
			return _seconds;
		}

		public Duration from_seconds ( int64 _seconds ) {
			return this;
		}

		public Duration from_string ( string str ) {
			return this;
		}
	}
}