namespace SqlPilot {
	public class FlightCrud : Crud {

		public FlightCrud ( Logbook logbook ) {
			base (logbook, "Flights");
		}

		public override Record new_record () {
			return new Flight ( this ) as Record;
		}
		public Flight beget () { return new_record () as Flight; }

 		public Flight? find_by_id (int64 id) {
			return (record_find_by_id (id) as Flight);
 		}

	}
}