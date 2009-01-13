using Sqlite;
namespace SqlPilot {
	public class RoleCrud : Crud {

		public RoleCrud ( Logbook logbook ) {
			base (logbook, "Roles");
		}
		
		public override Record new_record () {
			return new Role ( this );
		}
		public Role beget () { return new_record () as Role; }
		
 		public Role? find_by_id (int64 id) {
			return (record_find_by_id (id) as Role);
 		}



	}
}