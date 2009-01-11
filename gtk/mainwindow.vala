using GLib;
using Gtk;

namespace SqlPilot {
	public class MainWindow : Window {
		construct {
			this.title = "SqlPilot";
			this.destroy += Gtk.main_quit;
		}

		public void add_logbook (Logbook logbook) {
			add ( logbook.top_widget () );
		}
	}
}