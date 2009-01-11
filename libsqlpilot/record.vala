using Sqlite;
namespace SqlPilot {
	public class Record {
		
		public ulong id;
		public bool new_record;
		public bool changed;
		
		public virtual string to_string () { return ""; }
		
		public virtual bool save () { return false; }
		
		public virtual bool deletable () { return false; }
		
		public virtual bool is_valid () { return false; }
	}
}

	