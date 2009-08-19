using Sqlite;
using GLib;

namespace Sqlp {
	public class Transaction : Object {
		private static const string ta_begin_sql    = "SAVEPOINT s";
		private static const string ta_commit_sql   = "RELEASE s";
		private static const string ta_rollback_sql = "ROLLBACK TO s;";

		public unowned Database database { get; construct; }

		private Statement ta_begin;
		private Statement ta_commit;
		private Statement ta_rollback;

		private int nesting;

		public Transaction.on_database ( Database database ) {
			this.database = database;
		}

		construct {
 			nesting = 0;
			ta_begin = database.prepare_statement (ta_begin_sql);
			ta_commit = database.prepare_statement (ta_commit_sql);
			ta_rollback = database.prepare_statement (ta_rollback_sql);
		}

		public void begin () {
			ta_begin.step ();
			ta_begin.reset ();
			nesting++;
		}

		public void commit () {
			ta_commit.step ();
			ta_commit.reset ();
			nesting--;
		}

		public void rollback () {
			ta_rollback.step ();
			ta_rollback.reset ();
			nesting--;
		}
	}
}
