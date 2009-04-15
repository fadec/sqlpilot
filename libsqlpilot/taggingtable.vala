using Sqlite;
namespace Sqlp {
	public abstract class TaggingTable : Table <Logbook, Tagging> {
		public string object_id_column_name { get; construct; }
		public string tag_id_column_name { get; construct; }
	}
}
