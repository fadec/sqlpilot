using GLib;
using Sqlite;

namespace Sqlp {
	public abstract interface ITagging : Record {
		public abstract int64 object_id { get; set; }
		public abstract int64 tag_id { get; set; }
	}
}
