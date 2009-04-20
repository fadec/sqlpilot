using Sqlite;
namespace Sqlp {
	public class Crew : Record, ITagging  {

		public int64 object_id { get; set; }
		public int64 tag_id { get; set; }

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
			object_id = stmt.column_int64 (i++);
			tag_id = stmt.column_int64 (i++);
			role_id	= stmt.column_int64 (i++);
		}

		protected override int bind_for_save (Statement stmt) {
			var i = 1;
 			debug (object_id.to_string());
 			debug (tag_id.to_string());
			stmt.bind_nonzero_int64 (i++, object_id);
			stmt.bind_nonzero_int64 (i++, tag_id);
			stmt.bind_nonzero_int64	(i++, role_id);
			return i;
		}


	}
}
