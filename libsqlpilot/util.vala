using Sqlite;
namespace Sqlp {

	public static bool str_to_bool (string? s) {
		return (s != null && s[0] != 0 && s[0] != '0' && s[0] != 'F');
	}

	public static string empty_if_null (string? s) {
		return s != null ? s : "";
	}

// 	public static string builder_append_strings (StringBuilder sb, Iterable<string> strings, string spacer) {
// 		first = true;
// 		foreach (var s in strings) {
// 			if (! first) sb.append (spacer);
// 			else first = false;
// 			sb.append (s);
// 		}
// 	}

}