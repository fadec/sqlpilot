using GLib;
using Gtk;
using Sqlp;
using Sqlite;

namespace SqlpGtk {

	public class TagChooser : Pane {
		
		public unowned Sqlp.Table object_table { get; construct; }
		public unowned Sqlp.ITaggingTable tagging_table { get; construct; }
		public unowned Sqlp.Table tag_table { get; construct; }

		public string tag_header;
		public string tagging_header;

		public Statement find_taggings_by_object_id_stmt;

		private int64 _object_id;
		public int64 object_id {
			get { return _object_id; }
			set {
				_object_id = value;
				taggings.repopulate ();
				refilter_tags_view ();
			}
		}

		private enum TagColumn { TAG_ID }
		private TableObserverStore tags;
		private TreeModelFilter tags_filter;
		private TreeModelSort tags_sort;
		public TableView tags_view { get; private set; }

		private enum TaggingColumn { TAGGING_ID, OBJECT_ID, TAG_ID }
		private TableObserverStore taggings;
		public TableView taggings_view { get; private set; }

		private Button add_tagging_button;
		private Button remove_tagging_button;

		public TagChooser (Sqlp.Table object_table, Sqlp.ITaggingTable tagging_table, Sqlp.Table tag_table) {
			this.object_table = object_table;
			this.tagging_table = tagging_table;
			this.tag_table = tag_table;
			this.gui_name = "tag_chooser";
		}
		
		construct {
			Logbook l; // seems to fix (work around) vala header dependency bug
			Tag t;
			Tagging tt;
			add_tagging_button = gui.object ("add_tagging") as Button;
			add_tagging_button.clicked += on_add_tagging_button_clicked;
			remove_tagging_button = gui.object ("remove_tagging") as Button;
			remove_tagging_button.clicked += on_remove_tagging_button_clicked;

			tags = make_tags ();
			tags_view = make_tags_view (tags);
			taggings = make_taggings ();
			taggings_view = make_taggings_view (taggings);

			set_slot ("tags", tags_view);
			set_slot ("taggings", taggings_view);

			taggings.bind_scope = (stmt, i) => {
				stmt.bind_int64 (i++, object_id);
				return i;
			};
		}

		public void create_taggings_for_selected_tags () {}
		public void delete_selected_taggings () {}
		public void delete_selected_tags () {}

		private TableObserverStore make_tags () {
			//GLib.Type[] types = { typeof(uint64), typeof(string), typeof(string) };
			var tags = new TableObserverStore ();
			tags.database = tag_table.database;
			tags.select_sql = "SELECT * FROM " + tag_table.table_name;
			tags.observe (tag_table);
			return tags;
		}

		private TableView make_tags_view (TableObserverStore model) {
			var view = new TableView.with_model (model);
			return view;
		}

		private TableObserverStore make_taggings () {
			//GLib.Type[] types = { typeof(uint64), typeof(uint64), typeof(uint64), typeof(string), typeof(string) };
			var taggings = new TableObserverStore ();
			taggings.default_column_type = typeof(int64);
			taggings.database = tagging_table.database;
			taggings.id_column_name = "j.id";
			taggings.select_sql = find_taggings_by_object_id_select_sql ();
			taggings.scope_sql = find_taggings_by_object_id_scope_sql ();
			taggings.observe (tagging_table);
			return taggings;
		}

		private TableView make_taggings_view (TableObserverStore model) {
			return new TableView.with_model (model);
		}

		private string find_taggings_by_object_id_select_sql () {
			var s = new StringBuilder ();
			s.append ("SELECT j.id");
			s.append (" as id, j.");
			s.append (tagging_table.object_id_column_name);
			s.append (" as object_id, t.id");
			s.append (" as tag_id");
			for (int i=0; i < tag_table.column_count; i++) {
				s.append (", t.");
				s.append (tag_table.column_names[i]);
				s.append (" as ");
				s.append (tag_table.column_names[i]);
			}
			s.append (" FROM ");
			s.append (tag_table.table_name);
			s.append (" t INNER JOIN ");
			s.append (tagging_table.table_name);
			s.append (" j ON j.");
			s.append (tagging_table.tag_id_column_name);
			s.append (" = t.id");
			return s.str;
		}

		private string find_taggings_by_object_id_scope_sql () {
			return "j." + tagging_table.object_id_column_name + " = ?";
		}

		private void on_add_tagging_button_clicked (Button button) {
			var ids = tags_view.get_selected_ids ();
			foreach (var id in ids) {
				var tagging = tagging_table.new_record () as ITagging;
				tagging.tag_id = id;
				tagging.object_id = object_id;
				message (tagging.tag_id.to_string ());
				message (tagging.object_id.to_string ());
				tagging.save ();
			}
			refilter_tags_view ();
		}

		private void on_remove_tagging_button_clicked (Button button) {
			var ids = taggings_view.get_selected_ids ();
			foreach (var id in ids) {
				tagging_table.delete_id (id);
			}
			refilter_tags_view ();
		}

		private void refilter_tags_view () {
			var ids = taggings.get_int64_column_hashset (TaggingColumn.TAG_ID);
			tags_view.set_hidden_ids (ids);
		}
	}
}