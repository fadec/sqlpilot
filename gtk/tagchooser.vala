using GLib;
using Gtk;
using Sqlp;

namespace SqlpGtk {
	public class TagChooser : JoinTableEditor {
		public TagChooser (Sqlp.Table object_table, Sqlp.IJoinTable tagging_table, Sqlp.Table tag_table) {
			this.parent_table = object_table;
			this.join_table = tagging_table;
			this.child_table = tag_table;
			this.gui_name = "join_table_editor";
		}
	}
}
