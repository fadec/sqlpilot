using GLib;

namespace SqlpGtk {
	public class TagSchemaDescription : GLib.Object {
		public string object_table_name { get; set construct; }
		public string object_id_column { get; set construct; }

		public string tagging_table_name { get; set construct; }
		public string tagging_id_column { get; set construct; }
		public string tagging_object_id_column { get; set construct; }
		public string tagging_tag_id_column { get; set construct; }

		public string tag_table_name { get; set construct; }
		public string tag_id_column { get; set construct; }
		public string tag_name_column { get; set construct; }
		public string tag_description_column { get; set construct; }
	}
}