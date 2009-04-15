using Sqlp;
using Gtk;

namespace SqlpGtk {

	public class GlideEditor : Pane {

		public unowned Logbook logbook { private get; construct; }

		private int64 _flight_id;
		public int64 flight_id {
			get { return _flight_id; }
			set {
				_flight_id = value;
				glides.repopulate ();
			}
		}

		private Button glide_add;
		private Button glide_remove;
		private Label glide_summary;
		private TableView glides_view;
		private TableObserverStore glides;


		public GlideEditor (Logbook logbook) {
			this.logbook = logbook;
			this.gui_name = "glide_editor";
		}

		construct {
			glide_add = gui.object ("add") as Button;
			glide_remove = gui.object ("remove") as Button;
			glides = new TableObserverStore ();
			glides.select_sql = "SELECT * FROM Glides";
			glides.scope_sql = "flight_id = ?";
			glides.bind_scope = (stmt, i) => {
				stmt.bind_int64 (i++, flight_id);
				return i;
			};
			glides.database = logbook;
			glides.observe (logbook.glides);
			glides_view = new TableView.with_model (glides);
			glides_view.edited += on_glides_view_edited;	
			set_slot ("list_view", glides_view);
		}

		[CCode (instance_pos = -1)]
		public void on_add_clicked (Button button) {
			var glide = logbook.glides.new_record ();
			glide.flight = logbook.flight.find_by_id (flight_id);
			glide.save ();
		}

		[CCode (instance_pos = -1)]
		public void on_remove_clicked (Button button) {
			var ids = glides_view.get_selected_ids ();
			foreach (var id in ids) {
				logbook.glides.delete_id (id);
			}
		}

		private void on_glides_view_edited (TableView view, int64 id, string column_name, string new_text) {
			var glide = logbook.glides.find_by_id (id);
			switch (column_name) {
			case "Duration":
				glide.duration = Duration.from_string (new_text);
				break;
			case "Distance":
				glide.distance = new_text.to_double ();
				break;
			case "ReleaseAltitude":
				glide.release_altitude = new_text.to_double ();
				break;
			case "MaxAltitude":
				glide.max_altitude = new_text.to_double ();
				break;
			case "EngineStartAltitude":
				glide.engine_start_altitude = new_text.to_double ();
				break;
			}
			glide.save ();
		}

	}
}