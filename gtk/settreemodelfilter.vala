using GLib;
using Gtk;
using Gee;

namespace SqlpGtk {
	// Not really generic. It's for int64 only until I can fix is_in_visible_set ().
	class SetTreeModelFilter<T> : Gtk.TreeModelFilter {

		private HashSet<T>? _visible;
		public HashSet<T>? visible {
			get { return _visible; }
			set construct {
				_visible = value;
				refilter ();
			}
		}
		public int column { construct; get; }

		public SetTreeModelFilter (HashSet<T>? visible, int column, TreeModel child_model) {
			this.visible = visible;
			this.column = column;
			this.child_model = child_model;
		}

		construct {
			set_visible_func (is_in_visible_set);
		}

		[CCode (instance_pos = -1)]
		public bool is_in_visible_set (TreeModel model, TreeIter iter) {
			if (_visible == null) return true;
			int64 value; // should be type T but it segfaults
			model.get(iter, column, out value);
			return _visible.contains (&value);
		}


	}	
}