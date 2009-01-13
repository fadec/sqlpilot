using Sqlite;
namespace SqlPilot {
	public static void bind_nonempty_text_else_null (Statement stmt, int col, string str) {
		if (str.length > 0) stmt.bind_text (col, str);
		else stmt.bind_null (col);
	}

	public static void step_reset_clear (Statement stmt) {
		stmt.step ();
		stmt.reset ();
		stmt.clear_bindings ();
	}
}