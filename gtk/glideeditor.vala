using Sqlp;
using Gtk;

namespace SqlpGtk {

	public class GlideEditor : ParentChildEditor {

		public GlideEditor (Logbook logbook) {
			this.parent_table = logbook.flight;
			this.child_table = logbook.glides;
			this.child_view_name = "Glides";
			this.parent_id_fkey_column_name = "flight_id";
			this.gui_name = "parent_child_editor";
		}

		public override void associate_parent (Record child, int64 parent_id) {
			(child as Glide).flight = parent_table.find_by_id (parent_id) as Flight;
		}

		private override void on_table_view_edited (TableView view, int64 id, string column_name, string new_text) {
			var glide = child_table.find_by_id (id) as Glide;
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


		private virtual string summary_label_text () {
			return "33333";
		}
	}
}