using Sqlite;
namespace Sqlp {
	public class GlideTable : Table <Logbook, Glide> {
		
		public GlideTable (Logbook logbook) {
			this.record_type = typeof (Glide);
			this.database = logbook;
			this.table_name = "Glides";
		}

		construct {
		}

	}
}
