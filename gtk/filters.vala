using GLib;
using Gtk;
using Sqlite;

namespace SqlpGtk {
	public class Filters : Pane {
		public weak Browser browser;

		public signal void changed ();

		public string where_clause { private set; get; }

		private FilterTree filter_tree;
		private ToggleButton multiple_and;

		public Filters () {
			this.gui_name = "filters";
			this.multiple_and = gui.object ("multiple_and") as ToggleButton;
		}

		construct {
			this.filter_tree = new FilterTree ();
			set_slot ("filter_tree", filter_tree);
			filter_tree.selection_changed += something_has_indeed_changed;
		}

		private void something_has_indeed_changed () {
			string joiner = multiple_and.active ? " AND " : " OR ";
			var sb = new StringBuilder.sized (1024);
			bool first = true;
			foreach (string clause in filter_tree.where_clauses) {
				if (!first) {
					sb.append (joiner);
				} else first = false;
				sb.append (clause);
			}
			this.where_clause = sb.str;
			changed ();			
		}

		[CCode (instance_pos = -1)]
		public void on_multiple_and_toggled(ToggleButton button)
		{
			something_has_indeed_changed ();
		}

		[CCode (instance_pos = -1)]
		public void on_clear_clicked(Button button)
		{
			select_none ();
		}

		public void select_none () {
			filter_tree.select_none ();
		}

		public int count_selected_rows () {
			return filter_tree.count_selected_rows ();
		}

	}
}