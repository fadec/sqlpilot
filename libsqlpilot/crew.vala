using Sqlite;
namespace Sqlp {
	public class Crew : Record <ModelTable> {

		public override int bind_for_save (Statement stmt) {
			var i = 1;

			return i;
		}

		public override void set_from_stmt (Statement stmt) {
			var i = 1;
		}

	}
}
