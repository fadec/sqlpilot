using GLib;
using Gtk;
using Sqlp;
using Sqlite;

namespace SqlpGtk {

	public class TagManager : GLib.Object {
		
		public unowned Sqlp.Table <Logbook, Record> object_table { get; construct; }
		public unowned Sqlp.ITaggingTable tagging_table { get; construct; }
		public unowned Sqlp.TagTable tag_table { get; construct; }

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

		private enum TagColumn { TAG_ID, TAG_NAME, TAG_DESCRIPTION }
		private TableObserverStore tags;
		private TreeModelFilter tags_filter;
		private TreeModelSort tags_sort;
		public TableView tags_view { get; private set; }

		private enum TaggingColumn { TAGGING_ID, OBJECT_ID, TAG_ID, TAG_NAME, TAG_DESCRIPTION }
		private TableObserverStore taggings;
		public TableView taggings_view { get; private set; }

		private Button _add_tagging_button;
		public Button add_tagging_button {
			get { return _add_tagging_button; }
			set {
				_add_tagging_button = value;
				_add_tagging_button.clicked += on_add_tagging_button_clicked;
			}
		}
		private Button _remove_tagging_button;
		public Button remove_tagging_button {
			get { return _remove_tagging_button; }
			set {
				_remove_tagging_button = value;
				_remove_tagging_button.clicked += on_remove_tagging_button_clicked;
			}
		}

		private Button _add_tag_button;
		public Button add_tag_button {
			get { return _add_tag_button; }
			set {
				_add_tag_button = value;
				_add_tag_button.clicked += on_add_tag_button_clicked;
			}
		}
		private Button _remove_tag_button;
		public Button remove_tag_button {
			get { return _remove_tag_button; }
			set {
				_remove_tag_button = value;
				_remove_tag_button.clicked += on_remove_tag_button_clicked;
			}
		}

		
		public TagManager (Sqlp.Table object_table, Sqlp.ITaggingTable tagging_table, Sqlp.TagTable tag_table) {
			this.object_table = object_table;
			this.tagging_table = tagging_table;
			this.tag_table = tag_table;
		}
		
		construct {
			Logbook l; // seems to fix (work around) vala header dependency bug
			Tag t;
			Tagging tt;
			tags = make_tags ();
			tags_view = make_tags_view (tags);
			taggings = make_taggings ();
			taggings_view = make_taggings_view (taggings);

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
			view.edited += tags_view_edited;
			return view;
		}

		private void tags_view_edited (TableView tv, int64 id, string column_name, string new_text) {
			var tag = tag_table.find_by_id (id);
			switch (column_name) {
			case "Abbreviation":
				tag.abbreviation = new_text;
				break;
			case "Description":
				tag.description = new_text;
				break;
			}
			tag.save ();	
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
			foreach (string column_name in tag_table.column_names) {
				s.append (", t.");
				s.append (column_name);
				s.append (" as ");
				s.append (column_name);
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
				var tagging = tagging_table.new_record ();
				tagging.tag_id = id;
				tagging.object_id = object_id;
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

		private void on_add_tag_button_clicked (Button button) {
			var tag = tag_table.new_record ();
			tag.save ();
		}

		private void on_remove_tag_button_clicked (Button button) {
			var ids = tags_view.get_selected_ids ();
			foreach (var id in ids) {
				tag_table.delete_id (id);
			}
		}
		
	}
}