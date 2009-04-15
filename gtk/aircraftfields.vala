using Gtk;
using Gdk;
using Sqlp;

namespace SqlpGtk {

	public class AircraftFields : Fieldset <Role> {

		public AircraftFields (Sqlp.Table table) {
			this.gui_name = "aircraft_fields";
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
