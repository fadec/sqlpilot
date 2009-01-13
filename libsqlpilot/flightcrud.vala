namespace SqlPilot {
	public class FlightCrud : Crud {

		public FlightCrud ( Logbook logbook ) {
			find = logbook.prepare_statement ( "SELECT * FROM Flights WHERE id = ?;" );
		}

		public override Record new_record () {
			return new Flight ( this ) as Record;
		}

	}
}