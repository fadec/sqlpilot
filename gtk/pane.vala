using Gtk;
namespace SqlpGtk {
	// A Pane is a Component loaded from an Builder XML file.
	// The top widget must be called "top".
	// Slots are named containers.
	public class Pane : Component {
		
		public string gui_name { get; construct; }
		protected Gui gui;
		protected List<Component> children;

		public Pane ( string gui_name ) {
			this.gui_name = gui_name;
		}
		
		construct {
			gui = new Gui ( gui_name, this );
			children = new List<Component> ();

			if (! (gui.widget ( "top" ) is Widget ) ) {
				stderr.printf ( "no top widget\n" );
			} else {
				top_widget = gui.widget ( "top" );
			}
		}

		public void set_slot ( string slot_name, Component child ) {
			gui.box ( slot_name ).pack_start_defaults ( child.top_widget );
			children.append( child );
		}

	}

}