using Sqlite;
using GLib;

namespace Sqlp {
	public class Transaction : Object {
		private static const string ta_begin_sql    = "BEGIN TRANSACTION;";
		private static const string ta_commit_sql   = "COMMIT;";
		private static const string ta_rollback_sql = "ROLLBACK;";

		private weak Logbook logbook;

		private Statement ta_begin;
		private Statement ta_commit;
		private Statement ta_rollback;

		private int nesting;

		public Transaction ( Logbook logb ) {
			logbook = logb;
			nesting = 0;
			ta_begin = logbook.prepare_statement (ta_begin_sql);
			ta_commit = logbook.prepare_statement (ta_commit_sql);
			ta_rollback = logbook.prepare_statement (ta_rollback_sql);
		}

		public void begin () {
			if (nesting++ > 0) return;
			ta_begin.step ();
			ta_begin.reset ();
			nesting = 0;
		}

		public void commit () {
			if (--nesting > 0) return;
			ta_commit.step ();
			ta_commit.reset ();
			nesting = 0;
		}

		public void rollback () {
			if (--nesting > 0) return;
			ta_rollback.step ();
			ta_rollback.reset ();
			nesting = 0;
		}
	}
}
