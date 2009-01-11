using Gtk;
namespace SqlPilot {
	public class Pane {
		
		protected Gui gui;
		
		public Pane ( string filename ) {
			stderr.printf ("%s\n", filename );
			gui = new Gui ( filename );
		}

		public Widget top_widget () {
			if (! (gui.widget ( "top" ) is Widget ) ) stderr.printf ( "no top widget\n" );
			return gui.widget ( "top" );
		}

		public void add_child ( string slot_name, Pane child ) {
			gui.box ( slot_name ).pack_start_defaults ( child.top_widget () );
		}
	}

}