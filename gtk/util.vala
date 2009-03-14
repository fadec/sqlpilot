using GLib;
using Gtk;

namespace SqlpGtk {

	public static uint int64_hash (void *v) {
		return *((uint*) v);
	}

	public static bool int64_equal (void *a, void *b) {
		return *((int64*) a) == *((int64*) b);
	}

	public static bool select_entry (Entry e) {
		e.grab_focus ();
		e.select_region (0, -1);
		return false;
	}

}