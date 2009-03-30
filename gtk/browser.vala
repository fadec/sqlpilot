using GLib;
using Gtk;
using Sqlite;
using Sqlp;
using Gee;

namespace SqlpGtk {

	public class Browser : Pane {
		
		// owned by book.logbook
		// for crud operations
		public unowned Sqlp.Table <Sqlp.Database, Record> table { set construct; get; }

		private Fieldset <Record> _fieldset;
		public Fieldset <Record> fieldset {
			get { return _fieldset; }
			set construct {
				_fieldset = value;
				if (_fieldset == null) return;
				_fieldset.browser = this;
				set_fieldset_sensitivity ();
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

		private TableView _table_view;
		public TableView table_view {
			get { return _table_view; }
			set {
				_table_view = value;
				setup_table_view ();
				set_fieldset_sensitivity ();
			}
		}

		private bool _allow_delete;
		public bool allow_delete {
			get { return _allow_delete; }
			private set {
				_allow_delete = value;
				if (_allow_delete) {
					arm_delete_btn.sensitive = true;
					delete_btn.sensitive = true;
				} else {
					arm_delete_btn.active = false;
					arm_delete_btn.sensitive = false;
					delete_btn.sensitive = false;
				}
			}
		}

		private Entry where_clause;
		private Label query_error;
		private Label query_summary;
		private ToolButton add_btn;
		private ToolButton delete_btn;
		private ToggleToolButton arm_delete_btn;
		private Label record_summary;

		public Browser () {
			this.gui_name = "browser";
		}

		construct {
			where_clause		= gui.object ("where_clause")	as Entry;
			query_error			= gui.object ("query_error")	as Label;
			query_summary		= gui.object ("query_summary")	as Label;
			add_btn				= gui.object ("add")			as ToolButton;
			delete_btn			= gui.object ("delete")			as ToolButton;
			arm_delete_btn		= gui.object ("arm_delete")		as ToggleToolButton;
			record_summary = gui.object ("record_summary") as Label;
			

			this.filters = new Filters ();
			filters.changed += () => {
				where_clause.set_text (filters.where_clause);
				refilter ();
			};
			assert (this != null);
		}

		private void set_fieldset_sensitivity () {
			if (_fieldset == null) return;
			if (_table_view == null) {
				_fieldset.top_widget.sensitive = false;
				return;
			}
			_fieldset.top_widget.sensitive = (_table_view.count_selected_rows () == 1);
		}

		private void setup_table_view () {
			table_view.selection_changed += (table_view) => {
				if (fieldset.edited) fieldset.save ();

				if (table_view.count_selected_rows() == 1) {
					fieldset.record = table.find_by_id (table_view.get_selected_ids ()[0]);
					fieldset.top_widget.sensitive = true;
					record_summary.set_text (fieldset.record.summary ());
					this.allow_delete = fieldset.record.deletable ();
				} else {
					record_summary.set_text ("");
					fieldset.record = table.new_record ();
					fieldset.top_widget.sensitive = false;
					this.allow_delete = false;
				}
  			};
			set_slot ("table_view", table_view);
		}

		private virtual void refilter () {
			var ids = new HashSet <int64?> (int64_hash, int64_equal);
			var statement = table.database.prepare_statement (where_query_sql ());
			while (statement.step () == Sqlite.ROW) {
				ids.add (statement.column_int64 (0));
			}
			table_view.set_visible_ids (ids);
		}

		// for filters
 		private string where_query_sql () {
			string clause = where_clause.get_text ();
			string s = "SELECT id FROM (" + table_view.model.scoped_select_sql + ") WHERE " + (clause.length > 0 ? clause : "1=1");
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
			var selected_ids = table_view.get_selected_ids ();
			foreach (var id in selected_ids) {
				table.delete_id (id);
			}
			this.arm_delete_btn.active = false;
		}

		[CCode (instance_pos = -1)]
		public void on_add_clicked(ToolButton button)
		{
			var record = table.new_record ();
			// todo set default according to filters
			if (record.save ()) {
				// check visible and adjust filters
			}
		}
	}
}