using Sqlite;

namespace SqlPilot {

	public class NiceDB {

		private Database db;
		private static const string ta_begin    = "BEGIN TRANSACTION;";
		private static const string ta_commit   = "COMMIT;";
		private static const string ta_rollback = "ROLLBACK;";

		public NiceDB ( string filename ) throws NiceDBError {
			int code;
			code = Database.open(filename, out db);
			if (code != OK) {
				// pick any old error code for now
				throw new NiceDBError.ERROR ( "Some db error while opening" );
			}
			//db_register_sqlpilot_functions(db);
		}
	}

	public errordomain NiceDBError {
		// These do not correspond to the integer values of the native sqlite codes unfortunately
		    ERROR,
			INTERNAL,
			PERM,
			ABORT,
			BUSY,
			LOCKED,
			NOMEM,
			READONLY,
			INTERRUPT,
			IOERR,
			CORRUPT,
			NOTFOUND,
			FULL,
			CANTOPEN,
			PROTOCOL,
			EMPTY,
			SCHEMA,
			TOOBIG,
			CONSTRAINT,
			MISMATCH,
			MISUSE,
			NOLFS,
			AUTH,
			FORMAT,
			RANGE,
			NOTADB,
			ROW,
			DONE,
			}
}