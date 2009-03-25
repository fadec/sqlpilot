using GLib;
using Gtk;
using Sqlp;
using Sqlite;

namespace SqlpGtk {

	public class TagManager : GLib.Object {
		
		public unowned Sqlp.Database database;

		public string tag_header;
		public string tagging_header;

		public string object_table_name;
		public string tagging_table_name;
		public string tag_table_name;

		public string object_id_column;
		public string tagging_object_id_column;
		public string tagging_tag_id_column;
		public string tag_id_column;
		public string tag_name_column;
		public string tag_description_column;

		public Statement find_taggings_by_object_id_stmt;

		private uint64 _object_id;
		public uint64 object_id {
			get { return _object_id; }
			set { _object_id = value; }
		}

		// ListStore tags
		// uint64, string,   string
		// tag.id, tag.name, tag.description
		private enum TagColumn { TAG_ID, TAG_NAME, TAG_DESCRIPTION }
		private TableObserverStore tags;
		private TreeModelFilter tags_filter;
		private TreeModelSort tags_sort;
		private TreeView tags_view;

		// ListStore taggings
		// uint64,     uint64,            uint64,         string,   string
		// tagging.id, tagging.object_id, tagging.tag_id, tag.name, tag.description
		private enum TaggingColumn { TAGGING_ID, OBJECT_ID, TAG_ID, TAG_NAME, TAG_DESCRIPTION }
		private TableObserverStore taggings;
		private TreeView taggings_view;
		
		construct {
			object_table_name = "Models";
			tagging_table_name = "ModelTaggings";
			tag_table_name = "ModelTags";
			object_id_column = "id";
			tagging_object_id_column = "model_id";
			tagging_tag_id_column = "tag_id";
			tag_id_column = "id";
			tag_name_column = "Name";
			tag_description_column = "Description";
			find_taggings_by_object_id_stmt = database.prepare_statement (find_taggings_by_object_id_sql ());
			tags = make_tags ();
			tags_view = make_tags_view (tags);
			taggings = make_taggings ();
			taggings_view = make_taggings_view (taggings);

			taggings.bind_scope = 
		}

		public void create_taggings_for_selected_tags () {}
		public void delete_selected_taggings () {}
		public void delete_selected_tags () {}

		private TableObserverStore make_tags () {
			//GLib.Type[] types = { typeof(uint64), typeof(string), typeof(string) };
			var tags = new TableObserverStore ();
			tags.database = database;
			tags.select_sql = "SELECT id, Name, Description FROM FlightTags";

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
			taggings.database = database;
			taggings.select_sql = "SELECT ...";
			taggings.scope_sql = "object_id = ?";
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

		public string find_taggings_by_object_id_sql () {
			var s = new StringBuilder ();
			s.append ("SELECT t.");
			s.append (tag_id_column);
			s.append (", t.");
			s.append (tag_name_column);
			s.append (", t.");
			s.append (tag_description_column);
			s.append (" FROM ");
			s.append (tag_table_name);
			s.append (" t INNER JOIN ");
			s.append (tagging_table_name);
			s.append (" j ON j.");
			s.append (tagging_tag_id_column);
			s.append (" = t.");
			s.append (tag_id_column);
			s.append (" WHERE j.");
			s.append (tagging_object_id_column);
			s.append (" = ?");
			return s.str;
		}
		
	}
}