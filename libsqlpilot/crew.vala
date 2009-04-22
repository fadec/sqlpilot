using Sqlite;
namespace Sqlp {
	public class Crew : Record, IJoinRecord  {

		public int64 parent_id { get; set; }
		public int64 child_id { get; set; }

		public int64 role_id;
		private Role? _role;
		public Role? role {
			get {
				if (_role == null && role_id != 0) {
					_role = (table.database as Logbook).role.find_by_id (role_id) as Role?;
				}
				return _role;
			}
			set {
				_role = value;
				role_id = (value == null) ? 0 : value.id;
			}
		}

		protected override void set_from_stmt (Statement stmt) {
			var i = 1;
			parent_id = stmt.column_int64 (i++);
			child_id = stmt.column_int64 (i++);
			role_id	= stmt.column_int64 (i++);
		}

		protected override int bind_for_save (Statement stmt) {
			var i = 1;
			stmt.bind_nonzero_int64 (i++, parent_id);
			stmt.bind_nonzero_int64 (i++, child_id);
			stmt.bind_nonzero_int64	(i++, role_id);
			return i;
		}
	}
}
