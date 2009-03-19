using GLib;
using Gtk;
using Sqlite;
using Sqlp;
using Gee;

namespace SqlpGtk {

	public class Browser : Pane {
		
		// owns this
		public unowned Book book { construct; get; }

		// owned by book.logbook
		public unowned Sqlp.Table <Sqlp.Database, Record> table { construct; get; }

		private Fieldset _fieldset;
		public Fieldset fieldset {
			get { return _fieldset; }
			set construct {
				_fieldset = value;
				if (_fieldset == null) return;
				_fieldset.browser = this;
				set_slot ("fields", _fieldset);
				_fieldset.saved += on_fieldset_saved;
				_fieldset.top_widget.sensitive = (query_list.count_selected_rows () == 1);
			}
		}

		private Filters _filters;
		public Filters filters {
			get { return _filters; }
			set construct {
				_filters = value;
				if (_filters == null) return;
				_filters.browser = this;
				set_slot ("filters", _filters);
			}
		}

		public QueryList query_list { get; set; }
		public string view_name { get; construct; }

		private Record _current_record;
		private Record _copied_record;

		private TreeView _tree_view;
		private TreeSelection _tree_selection;
		private Action _new;
		private Action _save;
		private Action _cancel;
		private Action _arm_delete;
		private Action _delete;
		private Action _copy_record;
		private Action _paste_record;
		private Label  _message;

		private Entry where_clause;
		private Label query_error;
		private Label query_summary;
		private ToolButton add_btn;
		private ToolButton delete_btn;
		private ToggleToolButton arm_delete_btn;

		public Browser (string view_name, Sqlp.Table table, Book book) {
			this.gui_name = "browser";
			this.view_name = view_name;
			this.table = table;
			this.book = book;
		}

		construct {
			where_clause		= gui.object ("where_clause")	as Entry;
			query_error			= gui.object ("query_error")	as Label;
			query_summary		= gui.object ("query_summary")	as Label;
			add_btn				= gui.object ("add")			as ToolButton;
			delete_btn			= gui.object ("delete")			as ToolButton;
			arm_delete_btn		= gui.object ("arm_delete")		as ToggleToolButton;

			this.query_list = new QueryList (book.logbook, view_name);
			set_slot ("query_list", query_list);
   			query_list.selection_changed += (query_list) => {
				// attempt to save edited record ... e.g. text entries still in
				// focus but gtk wont fire focus-out-event for selection change.
				if (fieldset.edited) fieldset.save ();

				if (query_list.count_selected_rows() == 1) {
					fieldset.record = table.find_by_id (query_list.get_selected_ids ()[0]);
					fieldset.top_widget.sensitive = true;
				} else {
					fieldset.record = table.new_record ();
					fieldset.top_widget.sensitive = false;
				}
  			};
			this.filters = new Filters ();
			filters.changed += () => {
				where_clause.set_text (filters.where_clause);
				refilter ();
			};
			assert (this != null);
		}

		private void on_fieldset_saved (Fieldset fs, int64 id) {
			query_list.update_id (id);
		}

		private virtual void load_selection () {
		}

		private virtual void refilter () {
			var ids = new HashSet <int64?> (int64_hash, int64_equal);
			var statement = book.logbook.prepare_statement (where_query_sql ());
			while (statement.step () == Sqlite.ROW) {
				ids.add (statement.column_int64 (0));
			}
			query_list.set_visible_ids (ids);
		}

		// for filters
 		private string where_query_sql () {
			string clause = where_clause.get_text ();
			string s = "SELECT id FROM " + view_name + " WHERE " + (clause.length > 0 ? clause : "1=1") + ";";
			return s;
		}

		[CCode (instance_pos = -1)]
		public void on_refresh_clicked(ToolButton button)
		{
			refilter ();
		}

		[CCode (instance_pos = -1)]
		public void on_arm_delete_toggled(ToggleToolButton button)
		{
			this.delete_btn.sensitive = arm_delete_btn.active;
		}

		[CCode (instance_pos = -1)]
		public void on_delete_clicked(ToolButton button)
		{
			var selected_ids = query_list.get_selected_ids ();
			foreach (var id in selected_ids) {
				table.destroy_id (id);
			}
			query_list.remove_selected_ids ();
			this.arm_delete_btn.active = false;
		}

		[CCode (instance_pos = -1)]
		public void on_add_clicked(ToolButton button)
		{
			var record = table.new_record ();
			// todo set default according to filters
			if (record.save ()) {
				var iter = query_list.add_id (record.id);

 				if (! query_list.iter_is_visible (iter)) {
					where_clause.set_text ("");
					// being careful not to refilter twice - a change in selected rows will refilter via signals
					if (filters.count_selected_rows () > 0) {
						filters.select_none ();
					} else {
						refilter ();
					}
				}
				query_list.focus_iter (iter);
			}
		}
	}
}