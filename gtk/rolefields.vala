using Gtk;
using Gdk;
using Sqlp;

namespace SqlpGtk {

	public class RoleFields : Fieldset <Role> {

		private Entry abbreviation;
		private Entry name;
		private CheckButton total;
		private CheckButton pic;
		private CheckButton sic;
		private CheckButton flight_engineer;
		private CheckButton solo;
		private CheckButton dual_received;
		private CheckButton instructor;
		private CheckButton military;

		public RoleFields () {
			this.gui_name = "role_fields";
		}

		construct {
			abbreviation	= gui.object ("abbreviation")			 as Entry;
			name			= gui.object ("name")					 as Entry;
			total			= gui.object ("total")					 as CheckButton;
			pic				= gui.object ("pic")					 as CheckButton;
			sic				= gui.object ("sic")					 as CheckButton;
			flight_engineer = gui.object ("flight_engineer")		 as CheckButton;
			solo			= gui.object ("solo")					 as CheckButton;
			dual_received	= gui.object ("dual_received")			 as CheckButton;
			instructor		= gui.object ("instructor")				 as CheckButton;
			military		= gui.object ("military")				 as CheckButton;
		}

		protected override void set_fields_from_record () {
			assert (record is Record);
 			abbreviation.set_text (record.abbreviation != null ? record.abbreviation : "");
			name.set_text (record.name != null ? record.name : "");
 			total.active = record.total;
			pic.active = record.pic;
		}

		protected override void set_record_from_fields () {
		}


		[CCode (instance_pos = -1)]
		public void on_abbreviation_changed (Entry entry) {
			edited = true;
		}

		[CCode (instance_pos = -1)]
		public bool on_abbreviation_focus_out_event (Entry entry, EventFocus ev) {
			if (edited) {
				record.abbreviation = entry.get_text ();
				save ();
			}
			return false;
		}

		[CCode (instance_pos = -1)]
		public void on_name_changed (Entry entry) {
			edited = true;
		}

		[CCode (instance_pos = -1)]
		public bool on_name_focus_out_event (Entry entry, EventFocus ev) {
			if (edited) {
				record.name = entry.get_text ();
				save ();
			}
			return false;
		}

		[CCode (instance_pos = -1)]
		public void on_total_toggled (CheckButton cb) {
			record.total = cb.active;
			save ();
		}

		[CCode (instance_pos = -1)]
		public void on_pic_toggled (CheckButton cb) {
			record.pic = cb.active;
			save ();
		}

	}
}