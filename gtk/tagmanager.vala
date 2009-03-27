using GLib;
using Gtk;
using Sqlp;
using Sqlite;

namespace SqlpGtk {

	public class TagManager : GLib.Object {
		
		public unowned Sqlp.Table <Logbook, Record> object_table { get; construct; }
		public unowned Sqlp.Table <Logbook, Record> tagging_table { get; construct; }
		public unowned Sqlp.Table <Logbook, Record> tag_table { get; construct; }
		public TagSchemaDescription tsd { get; construct; }

		public string tag_header;
		public string tagging_header;

		public Statement find_taggings_by_object_id_stmt;

		private int64 _object_id;
		public int64 object_id {
			get { return _object_id; }
			set {
				_object_id = value;
				taggings.repopulate ();
			}
		}

		// ListStore tags
		// uint64, string,   string
		// tag.id, tag.name, tag.description
		private enum TagColumn { TAG_ID, TAG_NAME, TAG_DESCRIPTION }
		private TableObserverStore tags;
		private TreeModelFilter tags_filter;
		private TreeModelSort tags_sort;
		public TreeView tags_view { get; private set; }

		// ListStore taggings
		// uint64,     uint64,            uint64,         string,   string
		// tagging.id, tagging.object_id, tagging.tag_id, tag.name, tag.description
		private enum TaggingColumn { TAGGING_ID, OBJECT_ID, TAG_ID, TAG_NAME, TAG_DESCRIPTION }
		private TableObserverStore taggings;
		public TreeView taggings_view { get; private set; }

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
		
		public TagManager (Sqlp.Table object_table, Sqlp.Table tagging_table, Sqlp.Table tag_table, TagSchemaDescription tsd) {
			this.object_table = object_table;
			this.tagging_table = tagging_table;
			this.tag_table = tag_table;
			this.tsd = tsd;
		}
		
		construct {
			Logbook l; // seems to fix (work around) vala header dependency bug

			tags = make_tags ();
			tags_view = make_tags_view (tags);
			taggings = make_taggings ();
			taggings_view = make_taggings_view (taggings);

			taggings.bind_scope = (stmt) => {
				stmt.bind_int64 (1, object_id);
				return 1;
			};
		}

		public void create_taggings_for_selected_tags () {}
		public void delete_selected_taggings () {}
		public void delete_selected_tags () {}

		private TableObserverStore make_tags () {
			//GLib.Type[] types = { typeof(uint64), typeof(string), typeof(string) };
			var tags = new TableObserverStore ();
			tags.database = tag_table.database;
			tags.select_sql = "SELECT id, Name, Description FROM " + tsd.tag_table_name;
			tags.observe (tag_table);
			return tags;
		}

		private TreeView make_tags_view (TreeModel model) {
			var view = new TreeView.with_model (model);
			var names = new TreeViewColumn.with_attributes ("Name",
															new CellRendererText (),
															"text",
															TagColumn.TAG_NAME,
															null);
			names.set_sort_column_id (TagColumn.TAG_NAME);
			names.set_reorderable (true);
			names.set_resizable (true);
			view.insert_column (names, 0);
			var descriptions = new TreeViewColumn.with_attributes ("Description",
																   new CellRendererText (),
																   "text",
																   TagColumn.TAG_DESCRIPTION,
																   null);
			descriptions.set_sort_column_id (TagColumn.TAG_DESCRIPTION);
			descriptions.set_reorderable (true);
			descriptions.set_resizable (true);
			view.insert_column (descriptions, 1);
			return view;
		}

		private TableObserverStore make_taggings () {
			//GLib.Type[] types = { typeof(uint64), typeof(uint64), typeof(uint64), typeof(string), typeof(string) };
			var taggings = new TableObserverStore ();
			taggings.database = tagging_table.database;
			taggings.id_column_name = "t.id";
			taggings.select_sql = find_taggings_by_object_id_select_sql ();
			taggings.scope_sql = find_taggings_by_object_id_scope_sql ();
			taggings.observe (tagging_table);
			return taggings;
		}

		private TreeView make_taggings_view (TreeModel model) {
			var view = new TreeView.with_model (model);
			var names = new TreeViewColumn.with_attributes ("Name",
															new CellRendererText (),
															"text",
															TaggingColumn.TAG_NAME,
															null);
			names.set_sort_column_id (TaggingColumn.TAG_NAME);
			names.set_reorderable (true);
			names.set_resizable (true);
			view.insert_column (names, 0);
			var descriptions = new TreeViewColumn.with_attributes ("Description",
																   new CellRendererText (),
																   "text",
																   TaggingColumn.TAG_DESCRIPTION,
																   null);
			descriptions.set_sort_column_id (TaggingColumn.TAG_DESCRIPTION);
			descriptions.set_reorderable (true);
			descriptions.set_resizable (true);
			view.insert_column (descriptions, 1);
			return view;
		}

		private string find_taggings_by_object_id_select_sql () {
			var s = new StringBuilder ();
			s.append ("SELECT t.");
			s.append (tsd.tagging_id_column);
			s.append (", j.");
			s.append (tsd.tagging_object_id_column);
			s.append (", t.");
			s.append (tsd.tag_id_column);
			s.append (", t.");
			s.append (tsd.tag_name_column);
			s.append (", t.");
			s.append (tsd.tag_description_column);
			s.append (" FROM ");
			s.append (tsd.tag_table_name);
			s.append (" t INNER JOIN ");
			s.append (tsd.tagging_table_name);
			s.append (" j ON j.");
			s.append (tsd.tagging_tag_id_column);
			s.append (" = t.");
			s.append (tsd.tag_id_column);
			return s.str;
		}

		private string find_taggings_by_object_id_scope_sql () {
			return "j." + tsd.tagging_object_id_column + " = ?";
		}

		private void on_add_tagging_button_clicked (Button button) {
			message ("add");
		}
		private void on_remove_tagging_button_clicked (Button button) {
			message ("remove");
		}
		
	}
}