using GLib;
using Gtk;
using Sqlp;
using Sqlite;

namespace SqlpGtk {

	public class JoinTableEditor : Pane {
		
		public unowned Sqlp.Table parent_table { get; construct; }
		public unowned Sqlp.IJoinTable join_table { get; construct; }
		public unowned Sqlp.Table child_table { get; construct; }

		private int64 _parent_id;
		public int64 parent_id {
			get { return _parent_id; }
			set {
				_parent_id = value;
				join_records.repopulate ();
				refilter_children_view ();
			}
		}

		private Statement find_join_records_by_parent_id_stmt;

		private TableObserverStore children;
		private TreeModelFilter children_filter;
		private TreeModelSort children_sort;
		public TableView children_view { get; private set; }

		private enum JoinRecordColumn { JOIN_RECORD_ID, PARENT_ID, CHILD_ID }
		private TableObserverStore join_records;
		public TableView join_records_view { get; private set; }

		private Button add_join_record_button;
		private Button remove_join_record_button;

		public JoinTableEditor (Sqlp.Table parent_table, Sqlp.IJoinTable join_table, Sqlp.Table child_table) {
			this.parent_table = parent_table;
			this.join_table = join_table;
			this.child_table = child_table;
			this.gui_name = "join_table_editor";
		}
		
		construct {
			add_join_record_button = gui.object ("add_join_record") as Button;
			add_join_record_button.clicked += on_add_join_record_button_clicked;
			remove_join_record_button = gui.object ("remove_join_record") as Button;
			remove_join_record_button.clicked += on_remove_join_record_button_clicked;

			children = make_children ();
			children_view = make_children_view (children);
			join_records = make_join_records ();
			join_records_view = make_join_records_view (join_records);

			set_slot ("children", children_view);
			set_slot ("join_records", join_records_view);

			join_records.bind_scope = (stmt, i) => {
				stmt.bind_int64 (i++, parent_id);
				return i;
			};
		}

		private TableObserverStore make_children () {
			//GLib.Type[] types = { typeof(uint64), typeof(string), typeof(string) };
			var children = new TableObserverStore ();
			children.database = child_table.database;
			children.select_sql = "SELECT * FROM " + child_table.table_name;
			children.observe (child_table);
			return children;
		}

		private TableView make_children_view (TableObserverStore model) {
			var view = new TableView.with_model (model);
			return view;
		}

		private TableObserverStore make_join_records () {
			//GLib.Type[] types = { typeof(uint64), typeof(uint64), typeof(uint64), typeof(string), typeof(string) };
			var join_records = new TableObserverStore ();
			join_records.default_column_type = typeof(int64);
			join_records.database = join_table.database;
			join_records.id_column_name = "j.id";
			join_records.select_sql = find_join_records_by_parent_id_select_sql ();
			join_records.scope_sql = find_join_records_by_parent_id_scope_sql ();
			join_records.observe (join_table);
			return join_records;
		}

		private TableView make_join_records_view (TableObserverStore model) {
			return new TableView.with_model (model);
		}

		private string find_join_records_by_parent_id_select_sql () {
			var s = new StringBuilder ();
			s.append ("SELECT j.id");
			s.append (" as id, j.");
			s.append (join_table.parent_id_column_name);
			s.append (" as parent_id, t.id");
			s.append (" as child_id");
			for (int i=0; i < child_table.column_count; i++) {
				s.append (", t.");
				s.append (child_table.column_names[i]);
				s.append (" as ");
				s.append (child_table.column_names[i]);
			}
			s.append (" FROM ");
			s.append (child_table.table_name);
			s.append (" t INNER JOIN ");
			s.append (join_table.table_name);
			s.append (" j ON j.");
			s.append (join_table.child_id_column_name);
			s.append (" = t.id");
			return s.str;
		}

		private string find_join_records_by_parent_id_scope_sql () {
			return "j." + join_table.parent_id_column_name + " = ?";
		}

		private void on_add_join_record_button_clicked (Button button) {
			var ids = children_view.get_selected_ids ();
			foreach (var id in ids) {
				var join_record = join_table.new_record () as IJoinRecord;
				join_record.child_id = id;
				join_record.parent_id = parent_id;
				message (join_record.child_id.to_string ());
				message (join_record.parent_id.to_string ());
				join_record.save ();
			}
			refilter_children_view ();
		}

		private void on_remove_join_record_button_clicked (Button button) {
			var ids = join_records_view.get_selected_ids ();
			foreach (var id in ids) {
				join_table.delete_id (id);
			}
			refilter_children_view ();
		}

		private void refilter_children_view () {
			var ids = join_records.get_int64_column_hashset (JoinRecordColumn.CHILD_ID);
			children_view.set_hidden_ids (ids);
		}
	}
}
