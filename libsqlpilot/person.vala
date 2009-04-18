using Sqlite;
namespace Sqlp {
	public class Person : Record <RoleTable> {

		public string last_name { get; set; default = ""; }
		public string first_name { get; set; default = ""; }
		public string identification { get; set; default = ""; }
		public string notes { get; set; default = ""; }


		private List <Flight> _flights;
		public List <Flight> flights {
			get {
				if (_flights == null) {
//					_flights = table.database.flight.find_by_crew_person_id (id);
				}
				return _flights;
			}
		}

		private List <Crew> _crews;
		public List <Crew> crews {
			get {
				if (_crews == null) {
//					_crews = table.database.crew.find_by_person_id (id);
				}
				return _crews;
			}
		}

		public override string summary () {
			return "<Person %s, %s>".printf (last_name, first_name);
		}

		construct {
			message ("new role made");
		}

		protected override int bind_for_save (Statement stmt) {
			var i = 1;
			stmt.bind_nonempty_text (i++, last_name);
			stmt.bind_nonempty_text (i++, first_name);
			stmt.bind_nonempty_text (i++, identification);
			stmt.bind_nonempty_text (i++, notes);
			return i;
		}

		protected override void set_from_stmt (Statement stmt) {
			var i = 1;
			last_name = stmt.column_text(i++);
			first_name = stmt.column_text (i++);
			identification = stmt.column_text(i++);
			notes = stmt.column_text (i++);
		}

		public override bool deletable () {
			return crews.length () == 0;
		}

		protected override bool valid () {
// 			if (! is_unique_text(table.unique_abbreviation_stmt, abbreviation)) {
// 				return false;
// 			}
			return true;
		}
	}
}
