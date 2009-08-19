using GLib;
using Gtk;
using Sqlp;
using Gee;

namespace SqlpGtk {
	public class TakeoffView : TableView {


		private SetTreeModelFilter<int64?> airport_filter;

		// Selected flight.
		// Needed to filter airports
		private Flight _flight;
		public Flight flight {
			set {
				_flight = value;
				var ids = new HashSet <int64?> (int64_hash, int64_equal);
				ids.add (_flight.origin_id);
				ids.add (_flight.destination_id);
				
				var statement = this.store.database.prepare_statement ("SELECT airport_id from Routing WHERE flight_id = ?");
				statement.bind_int64 (1, _flight.id);
				while (statement.step () == Sqlite.ROW) {
					ids.add (statement.column_int64 (0));
				}

				airport_filter.function = SetTreeModelFilterFunction.SHOW;
				airport_filter.hashset = ids;
				airport_filter.refilter ();
			}
		}

 		public TakeoffView.with_model (TableObserverStore model) {
			this.model = model;
 		}

		public override void add_view_columns () {
			add_airport_column ();
			add_surface_column ();
			add_night_column ();
 			add_runway_column ();
   			add_crosswind_column ();
   			add_visibility_column ();
			add_aborted_column ();
		}


		private Takeoff takeoff_from_path (string path_string) {
			return (this.store.database as Logbook).takeoffs.find_by_id (get_id_at_path_string (path_string)) as Takeoff;
		}

		private Logbook logbook {
			get { return this.store.database as Logbook; }
		}

		private void add_airport_column () {
			var airport_store = new TableObserverStore ();
			airport_store.select_sql = "select id, COALESCE(ICAO, '') || '/' || COALESCE(IATA, '') || '/' || COALESCE(Abbreviation, '') as Ident FROM Airports";
			airport_store.database = this.store.database;
			airport_store.observe ((this.store.database as Logbook).airport);
			//airport_store.observe ((this.store.database as Logbook).flight, 1);
			var filter = new SetTreeModelFilter<int64?> (0, airport_store);
			var renderer = new CellRendererCombo ();
			renderer.model = filter;
			renderer.text_column = airport_store.column_offset ("Ident");
			renderer.has_entry = false;
			renderer.editable = true;

			renderer.changed.connect (on_airport_changed);

			var column = new TreeViewColumn.with_attributes ("Airport",
															 renderer,
															 "text",
															 store.column_offset ("Airport"));
			view.insert_column (column, 0);
			airport_filter = filter;
		}

		private void on_airport_changed (CellRendererCombo renderer, string path_string, TreeIter new_iter) {
			var filter = renderer.model as TreeModelFilter;
			var store = filter.child_model as TableObserverStore;
			TreeIter iter;
			filter.convert_iter_to_child_iter (out iter, new_iter);			
			int64 airport_id;
			store.get (iter, 0, out airport_id);

			var takeoff = takeoff_from_path (path_string);
			takeoff.airport = logbook.airport.find_by_id (airport_id) as Airport;
			takeoff.save ();
		}

		private void add_surface_column () {
			var surface_store = new TableObserverStore ();
			surface_store.select_sql = "SELECT id, Abbreviation FROM Surfaces";
			surface_store.database = this.store.database;
			surface_store.observe ((this.store.database as Logbook).surfaces);
			
			var renderer = new CellRendererCombo ();
			renderer.model = surface_store;
			renderer.text_column = surface_store.column_offset ("Abbreviation");
			renderer.has_entry = false;
			renderer.editable = true;

			renderer.changed.connect (on_surface_changed);

			var column = new TreeViewColumn.with_attributes ("Surface",
															 renderer,
															 "text",
															 store.column_offset ("Surface"));
			view.insert_column (column, 0);
		}

		private void on_surface_changed (CellRendererCombo renderer, string path_string, TreeIter new_iter) {
			var store = renderer.model as TableObserverStore;
			int64 surface_id;
			store.get (new_iter, 0, out surface_id);
			var takeoff = takeoff_from_path (path_string);
			takeoff.surface = logbook.surfaces.find_by_id (surface_id) as Surface;
			takeoff.save ();
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
			var takeoff = takeoff_from_path (path_string);
			takeoff.night = !takeoff.night;
			takeoff.save ();
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
			var takeoff = takeoff_from_path (path_string);
			takeoff.runway = new_text;
			takeoff.save ();
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
			var takeoff = takeoff_from_path (path_string);
			takeoff.crosswind = new_text.to_double ();
			takeoff.save ();
		}

		private void add_visibility_column () {
			var offset = store.column_offset ("Visibility");
			var renderer = new CellRendererText ();
			renderer.editable = true;
			renderer.edited.connect (on_visibility_edited);
			var column = new TreeViewColumn.with_attributes (store.column_names[offset],
															 renderer,
															 "text",
															 offset,
															 null);
			view.insert_column (column, offset);
		}

		private void on_visibility_edited (CellRendererText renderer, string path_string, string new_text) {
			var takeoff = takeoff_from_path (path_string);
			takeoff.visibility = new_text.to_double ();
			takeoff.save ();
		}

		private void add_aborted_column () {
			var offset = store.column_offset ("Aborted");
			var renderer = new CellRendererToggle ();
			renderer.toggled += on_aborted_toggled;
			var column = new TreeViewColumn.with_attributes (store.column_names[offset],
															 renderer,
															 "active",
															 offset,
															 null);
			view.insert_column (column, offset);
		}

   		private void on_aborted_toggled (string path_string) {
			var takeoff = takeoff_from_path (path_string);
			takeoff.aborted = !takeoff.aborted;
			takeoff.save ();
		}
		
	}
}
