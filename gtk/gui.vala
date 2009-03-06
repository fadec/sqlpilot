
using Gtk;
using GLib;
using Glade;

namespace SqlpGtk {
	public class Gui : GLib.Object {
		private static const string guidir = "data/ui/";
		private static GLib.Module? callback_module = null;
		
		private GLib.HashTable <string, Gtk.Object> objects;
		private string name;
		private void* callback_data;
		
		public Gui ( string guiname, void* callback_data = null ) {
			this.name = guiname;
			this.callback_data = callback_data;
			this.objects = new HashTable <string, Widget> (str_hash, str_equal);
			init_callback_module ();
			try {
				var builder = new Gtk.Builder ();
				builder.add_from_file ( guidir + name + ".xml" );
				weak SList<Gtk.Object> builder_objects = builder.get_objects ();
				foreach ( var obj in builder_objects ) {
					if ( obj is Widget ) {
						objects.insert ( (obj as Widget).name, (obj as Gtk.Object) );
//					stderr.printf ("--%s\n", (obj as Widget).name);
					}
				}
				builder.connect_signals_full (connect_callback_builder);
			}
			catch (Error e) {
				var glade_xml = new Glade.XML (guidir + name + ".xml", null, null);
				if (glade_xml != null) {
					weak List<Gtk.Widget> glade_widgets = glade_xml.get_widget_prefix ("");
					foreach ( var widget in glade_widgets ) {
						objects.insert (widget.name, widget as Gtk.Object);
					}
					glade_xml.signal_autoconnect_full (connect_callback_glade);
				} else {
					stderr.printf ( "%s\n", e.message );
					var msg = new MessageDialog ( null, DialogFlags.MODAL,
												  MessageType.ERROR, ButtonsType.CANCEL, 
												  "Failed to load UI\n%s", e.message );
					msg.run ();
				}
			}
		}

		public Gtk.Object? object (string name) {
			return this.objects.lookup (name) as Gtk.Object;
		}

		public Widget widget ( string name ) {
			var w = this.objects.lookup ( name );
			if ( w is Widget ) return ( w as Widget );
			else return ( w as Widget ); //should throw
		}

		public Entry entry ( string name ) {
			var e = this.objects.lookup ( name );
			assert (e is Entry);
			return (e as Entry);
		}

		public Box box ( string name ) {
			var b = objects.lookup ( name );
			return ( b as Box );
		}

		private void connect_callback_builder (Gtk.Builder builder,
									  GLib.Object object,
									  string signal_name,
									  string handler_name,
									  GLib.Object? connect_object,
									  GLib.ConnectFlags flags) {
			connect_callback (object, signal_name, handler_name);
		}

		private void connect_callback_glade (string handler_name,
											GLib.Object object,
											string signal_name,
											string? signal_data,
											GLib.Object? connect_object,
											bool after) {
			connect_callback (object, signal_name, handler_name);
		}

		private void connect_callback (GLib.Object object,
									  string signal_name,
									  string handler_name) {
			void* symbol;
			var full_symbol_name = "sqlp_gtk_" + this.name + "_" + handler_name;
			if (callback_module.symbol (full_symbol_name, out symbol)) {
				object.connect ("signal::" + signal_name, symbol, callback_data, null);
			} else {
				message ("no callback symbol: %s", full_symbol_name);
			}
		}

		private bool init_callback_module () {
			if (callback_module != null) return true;
			if (Module.supported () == false) return false;
			callback_module = Module.open (null, 0);
			return true;
		}
	}
}