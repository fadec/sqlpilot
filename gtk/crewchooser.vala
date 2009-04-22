using GLib;
using Gtk;
using Sqlp;

namespace SqlpGtk {
	public class CrewChooser : JoinTableEditor {

		public CrewChooser (Sqlp.Table object_table, Sqlp.IJoinTable tagging_table, Sqlp.Table tag_table) {
			this.parent_table = object_table;
			this.join_table = tagging_table;
			this.child_table = tag_table;
			this.gui_name = "join_table_editor";
		}

		construct {
			join_records_view.set_column_visible (0, false);
			join_records_view.set_column_visible (1, false);
			join_records_view.set_column_visible (2, false);
			join_records_view.set_column_visible (3, false);
			join_records_view.set_column_visible_by_title ("Notes", false);
			children_view.set_column_visible (0, false);
			children_view.set_column_visible_by_title ("Notes", false);
		}
	}
}
