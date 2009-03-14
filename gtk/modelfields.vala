using Gtk;
using Gdk;
using Sqlp;

namespace SqlpGtk {

	public class ModelFields : Fieldset <Role> {

		public ModelFields () {
			this.gui_name = "model_fields";
		}

		construct {
		}

		protected override void set_fields_from_record () {
		}

		protected override void set_record_from_fields () {
		}
	}
}
