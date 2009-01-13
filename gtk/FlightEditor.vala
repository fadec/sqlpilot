namespace SqlPilotGtk {
	public class FlightEditor : Editor {
		private weak Logbook logbook;

		public FlightEditor ( Logbook logb ) {
			base ( "data/ui/flight_fields_ga.xml", "data/ui/flight_toolbar.xml" );
			logbook = logb;
		}
	}
}