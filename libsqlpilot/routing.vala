using Sqlite;
namespace SqlPilot {
	public class Routing : Record {

		public Record flight;
		public Airport airport;
		public int seq;

		public Routing (RoutingCrud crud) {
			base (crud);
		}

		public override int bind_for_save (Statement stmt) {
			var i = 1;
			// bind here and return id column
			return i;
		}

		public override void set_from_stmt (Statement stmt) {
			var i = 0;
		}
	}
}