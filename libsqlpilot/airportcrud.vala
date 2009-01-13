namespace SqlPilot {
	public class AirportCrud : Crud {

		public AirportCrud ( Logbook logbook ) {
			base (logbook, "Airports");
		}

		public override Record new_record () {
			return new Airport ( this ) as Record;
		}
		public Airport beget () { return new_record () as Airport; }

 		public Airport? find_by_id (int64 id) {
			return (record_find_by_id (id) as Airport);
 		}

	}
}