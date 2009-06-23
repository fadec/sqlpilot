using GLib;
using Gtk;
using Sqlp;

namespace SqlpGtk {
	public class GlideView : TableView {

 		public GlideView.with_model (TableObserverStore model) {
			this.model = model;
 		}

		public override void add_view_columns () {
		}

		private Glide glide_from_path (string path_string) {
			return (this.store.database as Logbook).glides.find_by_id (get_id_at_path_string (path_string)) as Glide;
		}

	}
}
