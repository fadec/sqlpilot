using Sqlite;

namespace Sqlp {
	public class Aircraft : Record <AircraftTable> {

		public int64 model_id;
		private Model? _model;
		public Model? model {
			get {
				if (_model == null && model_id != 0) {
					_model = table.database.model.find_by_id (model_id);
				}
				return _model;
			}
			set {
				_model = value;
				model_id = (value == null) ? 0 : value.id;
			}
		}

		private string _tail = "";
		public string tail { get { return _tail; } set { _tail = value; is_modified = true; } }
		private string _fleetno = "";
		public string fleetno { get { return _fleetno; } set { _fleetno = value; is_modified = true; } }
		private string _notes = "";
		public string notes { get { return _notes; } set { _notes = value; is_modified = true; } }

		public override int bind_for_save (Statement stmt) {
			var i = 1;
			stmt.bind_int64 (i++, model_id);
			stmt.bind_nonempty_text (i++, tail);
			stmt.bind_nonempty_text (i++, fleetno);
			stmt.bind_nonempty_text (i++, notes);
			return i;
		}

		public override void set_from_stmt (Statement stmt) {
			var i = 1;
			model_id = stmt.column_int64 (i++);
			tail = stmt.column_text (i++);
			fleetno = stmt.column_text (i++);
			notes = stmt.column_text (i++);
		}

		protected override bool save_dependencies () {
			if (model != null && model.save ()) model_id = model.id;
			return true;
		}

		protected override bool save_dependents () {
			return true;
		}

	}

}