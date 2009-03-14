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
			sic.active = record.sic;
			flight_engineer.active = record.flight_engineer;
			solo.active = record.solo;
			dual_received.active = record.dual_received;
			instructor.active = record.instructor;
			military.active = record.military;
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
				if (! save ()) {
					Idle.add ( () => { select_entry (this.abbreviation); } );
				}
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

		[CCode (instance_pos = -1)]
		public void on_sic_toggled (CheckButton cb) {
			record.sic = cb.active;
			save ();
		}

		[CCode (instance_pos = -1)]
		public void on_flight_engineer_toggled (CheckButton cb) {
			record.flight_engineer = cb.active;
			save ();
		}

		[CCode (instance_pos = -1)]
		public void on_solo_toggled (CheckButton cb) {
			record.solo = cb.active;
			save ();
		}

		[CCode (instance_pos = -1)]
		public void on_dual_received_toggled (CheckButton cb) {
			record.dual_received = cb.active;
			save ();
		}

		[CCode (instance_pos = -1)]
		public void on_instructor_toggled (CheckButton cb) {
			record.instructor = cb.active;
			save ();
		}

		[CCode (instance_pos = -1)]
		public void on_military_toggled (CheckButton cb) {
			record.military = cb.active;
			save ();
		}

	}
}