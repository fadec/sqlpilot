
using Gtk;
using GLib;

namespace SqlPilot {
	public class Gui {
		private HashTable <string, Gtk.Object> objects;

		public Gui ( string filename ) {
			objects = new HashTable <string, Widget> (str_hash, str_equal);
			var builder = new Gtk.Builder ();
			try {
				builder.add_from_file ( filename );
			} catch (Error e) {
				stderr.printf ( "%s\n", e.message );
				var msg = new MessageDialog ( null, DialogFlags.MODAL,
											  MessageType.ERROR, ButtonsType.CANCEL, 
											  "Failed to load UI\n%s", e.message );
				msg.run ();
			}
			weak SList<Gtk.Object> builder_objects = builder.get_objects ();
			foreach ( var obj in builder_objects ) {
				if ( obj is Widget ) {
				   	objects.insert ( (obj as Widget).name, (obj as Gtk.Object) );
					//stderr.printf ("--%s\n", (obj as Widget).name);
				}
			}
		}

		public Widget widget ( string name ) {
			var w = objects.lookup ( name );
			if ( w is Widget ) return ( w as Widget );
			else return ( w as Widget ); //should throw
		}

		public Box box ( string name ) {
			var b = objects.lookup ( name );
			return ( b as Box );
		}
	}
}