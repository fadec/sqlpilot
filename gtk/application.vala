using GLib;
using Gtk;
//using Gnome;

namespace SqlpGtk {
	public class Application : Pane {

		private SqlpilotAboutDialog? about_dialog;

		public Application () {
			gui_name = "application";
		}

		construct {
			top_widget.destroy += Gtk.main_quit;
		}

		public void add_book (Book book) {
			set_slot ("content", book );
		}

		public void clean_exit () {
			Gtk.main_quit ();
		}

		[CCode (instance_pos = -1)]
		public void on_file_new_activate (Entry entry) {
			message ("Yay activate\n");
		}

		[CCode (instance_pos = -1)]
		public void on_about_activate (MenuItem item) {
 			if (about_dialog == null) about_dialog = new SqlpilotAboutDialog (this.top_widget as Window);
 			about_dialog.present ();
		}

		[CCode (instance_pos = -1)]
		public void on_quit_activate (MenuItem item) {
			clean_exit ();
		}




	}
}