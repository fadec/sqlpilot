using GLib;
using Gtk;
using Sqlite;

namespace SqlpGtk {
	public class Filters : Pane {
		public weak Browser browser;

		private FilterTree filter_tree;

		public Filters () {
			this.gui_name = "filters";
		}

		construct {
			this.filter_tree = new FilterTree ();
			set_slot ("filter_tree", filter_tree);
		}

	}
}