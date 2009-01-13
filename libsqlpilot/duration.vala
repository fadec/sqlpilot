
namespace SqlPilot {

	public class Duration {
		private int64 _seconds;
		
		public int64 to_seconds () {
			return _seconds;
		}

		public Duration set_from_seconds ( int64 _seconds ) {
			return this;
		}
	}
}