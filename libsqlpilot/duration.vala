using Sqlite;

namespace Sqlp {

	public struct Duration {
		private long _seconds;
		private bool _valid;

		public Duration () {
			_seconds = 0;
			_valid = false;
		}

		public Duration.from_seconds ( long seconds ) {
			_seconds = seconds;
			_valid = true;
		}
		public Duration.from_minutes ( long minutes ) {
			_seconds = minutes * 60;
			_valid = true;
		}

		public Duration.from_string ( string str ) {
			if (str != null && str.validate () && str[0] != '\0') {
				int a = 0;
				int b = 0;
				var matched = str.scanf ("%d+%d", out a, out b);
				_seconds = (matched - 1) * 3600 * a + 60 * b;
				_valid = true;
			} else {
				_seconds = 0;
				_valid = false;
			}
		}

		public time_t to_time_t () {
			// should throw if not valid
			return (time_t) _seconds;
		}

		public long to_seconds () {
			// should throw if not valid
			return _seconds;
		}

		public long to_minutes () {
			// should throw if not valid
			return _seconds / 60;
		}

		public long to_hours () {
			// should throw if not valid
			return _seconds / 3600;
		}

		public string to_string () {
			if (valid ()) {
				var s = "%d+%02d".printf ((int)(_seconds / 3600), (int)((_seconds / 60) % 60));
				return s;
			} else {
				return "";
			}
		}

		public bool valid () {
			return _valid;
		}

		public void bind_to_stmt (Statement stmt, int index) {
			if (valid ()) stmt.bind_int64 (index, _seconds);
			else stmt.bind_null (index);
		}

		public Duration.from_stmt (Statement stmt, int offset) {
			if (stmt.column_type (offset) == Sqlite.NULL) {
				_seconds = 0;
				_valid = false;
			} else {
				_seconds = stmt.column_int (offset);
				_valid = true;
			}
		}

	}
}