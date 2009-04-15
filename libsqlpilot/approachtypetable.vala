using Sqlite;
namespace Sqlp {
	public class ApproachTypeTable : Table <Logbook, ApproachType> {

		public ApproachTypeTable ( Logbook logbook ) {
			this.record_type = typeof (ApproachType);
			this.database = logbook;
			this.table_name = "ApproachTypes";
		}

	}
}