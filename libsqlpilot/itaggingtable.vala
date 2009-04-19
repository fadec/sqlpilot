using Sqlite;
namespace Sqlp {
	public abstract interface ITaggingTable : Table <Logbook, Tagging> {
		public abstract string object_id_column_name { get; }
		public abstract string tag_id_column_name { get; }
	}
}
