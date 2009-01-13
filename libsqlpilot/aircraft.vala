using Sqlite;

namespace SqlPilot {
	public class Aircraft : Record {

		public int64 model_id;
		private Model? _model;
		public Model? model {
			get {
				if (_model == null && model_id != 0) {
					_model = crud.logbook.model.find_by_id (model_id);
				}
				return _model;
			}
			set {
				_model = value;
				model_id = value.id;
			}
		}

		public string tail = "";
		public string fleetno = "";
		public string notes = "";

		public Aircraft (AircraftCrud crud) {
			base (crud);
		}

		public override int bind_for_save (Statement stmt) {
			var i = 1;
			stmt.bind_int64 (i++, model_id);
			bind_nonempty_text_else_null (stmt, i++, tail);
			bind_nonempty_text_else_null (stmt, i++, fleetno);
			bind_nonempty_text_else_null (stmt, i++, notes);
			return i;
		}

		public override void set_from_stmt (Statement stmt) {
			var i = 1;
			model_id = stmt.column_int64 (i++);
			tail = stmt.column_text (i++);
			fleetno = stmt.column_text (i++);
			notes = stmt.column_text (i++);
		}

	}

}