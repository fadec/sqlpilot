using GLib;
using Gtk;
namespace SqlpGtk {
	// A component contains the callbacks for a subtree of widgets
	public abstract class Component : GLib.Object {
		public Widget top_widget { get; protected set; }
	}
}