using GLib;
using Gtk;
using Sqlp;

namespace SqlpGtk {
	public class LandingView : TableView {

 		public LandingView.with_model (TableObserverStore model) {
			this.model = model;
 		}

		public override void add_view_columns () {
			add_airport_column ();
			add_surface_column ();
			add_night_column ();
			add_runway_column ();
			add_crosswind_column ();
			add_full_stop_column ();
			add_night_vision_column ();
			add_autoland_column ();
		}

		private Landing landing_from_path (string path_string) {
			return (this.store.database as Logbook).landings.find_by_id (get_id_at_path_string (path_string)) as Landing;
		}

		private void add_airport_column () {
		}

		private void add_surface_column () {
		}

		private void add_night_column () {
			var offset = store.column_offset ("Night");
			var renderer = new CellRendererToggle ();
			renderer.toggled += on_night_toggled;
			var column = new TreeViewColumn.with_attributes (store.column_names[offset],
															 renderer,
															 "active",
															 offset,
															 null);
			view.insert_column (column, offset);
		}

   		private void on_night_toggled (string path_string) {
			var landing = landing_from_path (path_string);
			landing.night = !landing.night;
			landing.save ();
		}

		private void add_runway_column () {
			var offset = store.column_offset ("Runway");
			var renderer = new CellRendererText ();
			renderer.editable = true;
			renderer.edited.connect (on_runway_edited);
			var column = new TreeViewColumn.with_attributes (store.column_names[offset],
															 renderer,
															 "text",
															 offset,
															 null);
			view.insert_column (column, offset);
		}

		private void on_runway_edited (CellRendererText renderer, string path_string, string new_text) {
			var landing = landing_from_path (path_string);
			landing.runway = new_text;
			landing.save ();
		}

		private void add_crosswind_column () {
			var offset = store.column_offset ("Crosswind");
			var renderer = new CellRendererText ();
			renderer.editable = true;
			renderer.edited.connect (on_crosswind_edited);
			var column = new TreeViewColumn.with_attributes (store.column_names[offset],
															 renderer,
															 "text",
															 offset,
															 null);
			view.insert_column (column, offset);
		}

		private void on_crosswind_edited (CellRendererText renderer, string path_string, string new_text) {
			var landing = landing_from_path (path_string);
			landing.crosswind = new_text.to_double ();
			landing.save ();
		}

		private void add_full_stop_column () {
			var offset = store.column_offset ("FullStop");
			var renderer = new CellRendererToggle ();
			renderer.toggled += on_full_stop_toggled;
			var column = new TreeViewColumn.with_attributes (store.column_names[offset],
															 renderer,
															 "active",
															 offset,
															 null);
			view.insert_column (column, offset);
		}

   		private void on_full_stop_toggled (string path_string) {
			var landing = landing_from_path (path_string);
			landing.full_stop = !landing.full_stop;
			landing.save ();
		}

		private void add_night_vision_column () {
			var offset = store.column_offset ("NightVision");
			var renderer = new CellRendererToggle ();
			renderer.toggled += on_night_vision_toggled;
			var column = new TreeViewColumn.with_attributes (store.column_names[offset],
															 renderer,
															 "active",
															 offset,
															 null);
			view.insert_column (column, offset);
		}

   		private void on_night_vision_toggled (string path_string) {
			var landing = landing_from_path (path_string);
			landing.night_vision = !landing.night_vision;
			landing.save ();
		}

		private void add_autoland_column () {
			var offset = store.column_offset ("Autoland");
			var renderer = new CellRendererToggle ();
			renderer.toggled += on_autoland_toggled;
			var column = new TreeViewColumn.with_attributes (store.column_names[offset],
															 renderer,
															 "active",
															 offset,
															 null);
			view.insert_column (column, offset);
		}

   		private void on_autoland_toggled (string path_string) {
			var landing = landing_from_path (path_string);
			landing.autoland = !landing.autoland;
			landing.save ();
		}

	}
}
