using Sqlite;
namespace SqlPilot {

	public static bool str_to_bool (string? s) {
		return (s != null && s[0] != 0 && s[0] != '0' && s[0] != 'F');
	}

}