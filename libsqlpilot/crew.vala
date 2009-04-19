using Sqlite;
namespace Sqlp {
	public class Crew : Record <CrewTable> {

		public int64 object_id { get; set; }
		public int64 tag_id { get; set; }

		public int64 role_id;
		private Role? _role;
		public Role? role {
			get {
				if (_role == null && role_id != 0) {
					_role = table.database.role.find_by_id (role_id);
				}
				return _role;
			}
			set {
				_role = value;
				role_id = (value == null) ? 0 : value.id;
			}
		}

		protected override int bind_for_save (Statement stmt) {
			var i = 1;
			object_id = stmt.column_int64 (i++);
			tag_id = stmt.column_int64 (i++);
			stmt.bind_nonzero_int64				(i++, role_id);
			return 123;
		}

		protected override void set_from_stmt (Statement stmt) {
			var i = 1;
			object_id = stmt.column_int64 (i++);
			tag_id = stmt.column_int64 (i++);
			role_id	= stmt.column_int64 (i++);
		}

	}
}
