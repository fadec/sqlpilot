using Gtk;
using Gdk;
using Sqlp;

namespace SqlpGtk {

	public class AirportFields : Fieldset <Role> {

		private Entry abbreviation;
		private Entry icao;
		private Entry iata;
		private Entry name;

		public AirportFields (Sqlp.Table table) {
			this.gui_name = "airport_fields";
			this.table = table;
		}

		construct {
		}

		protected override void set_fields_from_record () {
		}

		protected override void set_record_from_fields () {
		}
	}
}
