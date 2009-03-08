using GLib;
using Gtk;
using Sqlite;
using Sqlp;
using Gee;

namespace SqlpGtk {

	public class Browser : Pane {

		private enum State {
			EMPTY,		   // --> treeselect->SELECTED | editing->MODIFIED | editing->INVALID | paste->(MODIFIED | INVALID)
			SELECTED,	   // --> cancel->EMPTY | editing->MODIFIED | editing->INVALID | arm->DELETE_ARMED | copy | paste->(MODIFIED | INVALID)
			MULTIPLE,	   // --> arm->DELETE_ARMED
			MODIFIED,      // --> save->SELECTED | cancel->(SELECTED | EMPTY) | copy | paste->(MODIFIED | INVALID)
			INVALID,	   // --> cancel->SELECTED | cancel->EMPTY | copy | paste->(MODIFIED | INVALID)
			DELETE_ARMED,  // --> unarm->(SELECTED | MULTIPLE) | delete->EMPTY
		}
		
		// owns this
		public weak Book book { construct; get; }

		// owned by book.logbook
		// Record is base type. Will correct destructor be called for e.g. Flight?
		// Apparently so, since it uses g_object_unref as a destructor
		public weak Crud<Record> crud { construct; get; }

		private Fieldset _fieldset;
		public Fieldset fieldset {
			get { return _fieldset; }
			set construct {
				_fieldset = value;
				if (_fieldset == null) return;
				_fieldset.browser = this;
				set_slot ("fields", _fieldset);
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

		private State _state;
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

		public Browser (string view_name, Crud crud, Book book) {
			this.gui_name = "browser";
			this.view_name = view_name;
			this.crud = crud;
			this.book = book;
		}

		construct {
			where_clause		= gui.object ("where_clause")	as Entry;
			query_error			= gui.object ("query_error")	as Label;
			query_summary		= gui.object ("query_summary")	as Label;
			add_btn				= gui.object ("add")			as ToolButton;
			delete_btn			= gui.object ("delete")			as ToolButton;
			arm_delete_btn		= gui.object ("arm_delete")		as ToggleToolButton;

			var query_statement = prepare_query_statement ();
			this.query_list = new QueryList ((owned)query_statement);
			set_slot ("query_list", query_list);
   			query_list.selection_changed += (query_list) => {
				if (query_list.count_selected_rows() == 1)
					fieldset.record = crud.find_by_id (query_list.get_selected_ids ()[0]);
				else fieldset.record = crud.new_record ();
  			};
			this.filters = new Filters ();
			filters.changed += () => {
				where_clause.set_text (filters.where_clause);
				refilter ();
			};
		}

		private Statement prepare_query_statement () {
			var stmt = book.logbook.prepare_statement (
				"SELECT * FROM " + this.view_name + ";");
			return stmt;
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

		
		private virtual void save () {
			_current_record.save();
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
			this.arm_delete_btn.active = false;
		}

		[CCode (instance_pos = -1)]
		public void on_add_clicked(ToolButton button)
		{
			var record = crud.new_record ();
			// todo set default according to filters
			if (record.save ()) {
				var sql = "SELECT * FROM " + view_name + " WHERE id = ?;";
				var select_statement = book.logbook.prepare_statement (sql);
				select_statement.bind_int64 (1, record.id);

				// This is totally redundant but seems to sync things for iterators.
				// need to fix
				refilter ();

				var iter = query_list.append (select_statement);

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

// 		[CCode (instance_pos = -1)]
// 		public void on_save_btn_clicked(Button button)
// 		{
// 			save ();
// 		}

	}
}