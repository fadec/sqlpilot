using Sqlite;
namespace Sqlp {
	public class LaunchTypeTable : Table {

		public LaunchTypeTable ( Logbook logbook ) {
			this.record_type = typeof (LaunchType);
			this.database = logbook;
			this.table_name = "LaunchTypes";
		}

	}
}