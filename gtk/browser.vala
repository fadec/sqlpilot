using GLib;
using Gtk;
using Sqlite;
using Sqlp;

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
		public weak Crud crud { construct; get; }

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

		public Browser (string view_name, Crud crud, Book book) {
			this.gui_name = "browser";
			this.view_name = view_name;
			this.crud = crud;
			this.book = book;
		}

		construct {
			var query_statement = prepare_query_statement ();
			this.query_list = new QueryList ((owned)query_statement);
			set_slot ("query_list", query_list);

			this.filters = new Filters ();
		}

		private Statement? prepare_query_statement () {
			var where_clause = "1 = 1";
			var stmt = book.logbook.prepare_statement (
				"SELECT * FROM " + this.view_name + " WHERE " + where_clause + ";");
			return stmt;
		}

		private virtual void load_selection () {
		}

		private virtual void refresh () {
			
		}
		
		private virtual void save () {
			_current_record.save();
		}

		[CCode (instance_pos = -1)]
		public void on_armdel_btn_toggled(ToggleButton button)
		{
		}

		[CCode (instance_pos = -1)]
		public void on_del_btn_clicked(Button button)
		{
		}

		[CCode (instance_pos = -1)]
		public void on_new_btn_clicked(Button button)
		{
		}

		[CCode (instance_pos = -1)]
		public void on_save_btn_clicked(Button button)
		{
			save ();
		}

	}
}