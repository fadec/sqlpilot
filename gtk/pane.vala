using Gtk;
namespace SqlPilotGtk {
	public class Pane {
		
		protected Gui gui;
		protected List<Pane> children;

		public Pane ( string name ) {
			stderr.printf ("%s\n", name );
			gui = new Gui ( name, this );
			children = new List<Pane> ();
		}

		public Widget top_widget () {
			if (! (gui.widget ( "top" ) is Widget ) ) stderr.printf ( "no top widget\n" );
			return gui.widget ( "top" );
		}

		public void add_child ( string slot_name, Pane child ) {
			gui.box ( slot_name ).pack_start_defaults ( child.top_widget () );
			children.append( child );
		}

	}

}