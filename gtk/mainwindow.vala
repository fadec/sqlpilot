using GLib;
using Gtk;

namespace SqlpGtk {
	public class MainWindow : Window {
		construct {
			this.title = "Sqlpilot";
			this.destroy += Gtk.main_quit;
		}

		public void add_book (Book book) {
			add ( book.top_widget );
		}
	}
}