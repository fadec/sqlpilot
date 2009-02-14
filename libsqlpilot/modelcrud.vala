using Sqlite;
namespace Sqlp {
	public class ModelCrud : Crud {

		private Statement find_by_ident_stmt;

		public ModelCrud ( Logbook logbook ) {
			base (logbook, "Models");
			var find_by_ident_sql = "SELECT * FROM Models WHERE Ident = ?;";
			find_by_ident_stmt = logbook.prepare_statement (find_by_ident_sql);
		}

		public override Record new_record () {
			return new Model ( this ) as Record;
		}
		public Model beget () { return new_record () as Model; }

 		public Model? find_by_id (int64 id) {
			return (record_find_by_id (id) as Model);
 		}

		public Model? find_by_ident (string ident) {
			find_by_ident_stmt.bind_text (1, ident);
			return (record_find_first (find_by_ident_stmt) as Model);
		}


	}
}