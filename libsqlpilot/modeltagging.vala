using Sqlite;
namespace Sqlp {
	public class ModelTagging : Record <ModelTaggingTable> {
		protected override void set_from_stmt (Statement stmt) {}
		protected override int bind_for_save (Statement stmt) {
			return 2;
		}
	}
}
