using GLib;
using Gtk;
using Gee;

namespace SqlpGtk {
	// Not really generic. It's for int64 only until I can fix is_in_visible_set ().
	public enum SetTreeModelFilterFunction { IGNORE, HIDE, SHOW }
	class SetTreeModelFilter<T> : Gtk.TreeModelFilter {

		private HashSet<T>? _hashset;
		public HashSet<T>? hashset {
			get { return _hashset; }
			set construct {
				_hashset = value;
			}
		}

		public int column { set construct; get; default = 0; }

		public SetTreeModelFilterFunction function { get; set; default = SetTreeModelFilterFunction.HIDE; }

		public SetTreeModelFilter (int column, TreeModel child_model) {
			this.child_model = child_model;
			this.column = column;
		}

		construct {
			set_visible_func (the_filter);
		}

		[CCode (instance_pos = -1)]
		public bool the_filter (TreeModel model, TreeIter iter) {
			if (_hashset == null || function == SetTreeModelFilterFunction.IGNORE) return true;
			int64 value; // should be type T but it segfaults
			model.get(iter, column, out value);
			var found = _hashset.contains (&value);
			return function == SetTreeModelFilterFunction.HIDE ? !found : found;
		}
	}	
}