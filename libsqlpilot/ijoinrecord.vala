using GLib;

namespace Sqlp {
	public abstract interface IJoinRecord : Record {
		public abstract int64 parent_id { get; set; }
		public abstract int64 child_id { get; set; }
	}
}
