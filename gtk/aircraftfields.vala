using Gtk;
using Gdk;
using Sqlp;

namespace SqlpGtk {

	public class AircraftFields : Fieldset <Role> {

		public AircraftFields () {
			this.gui_name = "aircraft_fields";
		}

		construct {
		}

		protected override void set_fields_from_record () {
		}

		protected override void set_record_from_fields () {
		}
	}
}
