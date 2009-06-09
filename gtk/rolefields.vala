using Gtk;
using Gdk;
using Sqlp;

namespace SqlpGtk {

	public class RoleFields : Fieldset {

		private Entry abbreviation;
		private Entry description;
		private CheckButton total;
		private CheckButton pic;
		private CheckButton sic;
		private CheckButton flight_engineer;
		private CheckButton solo;
		private CheckButton dual_received;
		private CheckButton instructor;
		private CheckButton military;

		private TagChooser property_chooser;

		public Role role {
			get { return this.record as Role; }
			set { this.record = value; }
		}

		public RoleFields (Sqlp.Table table) {
			this.gui_name = "role_fields";
			this.table = table;
		}

		construct {
			var logbook = table.database as Logbook;

			abbreviation	= gui.object ("abbreviation")			 as Entry;
			description		= gui.object ("description")			 as Entry;

			property_chooser = new TagChooser (table, logbook.role_taggings, logbook.role_tags);
			set_slot ("property_chooser", property_chooser);
		}

		protected override void set_fields_from_record () {
 			abbreviation.set_text (role.abbreviation != null ? role.abbreviation : "");
			description.set_text (role.description != null ? role.description : "");
			property_chooser.parent_id = role.id;
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
				role.abbreviation = entry.get_text ();
				if (! save ()) {
					Idle.add ( () => { select_entry (this.abbreviation); } );
				}
			}
			return false;
		}

		[CCode (instance_pos = -1)]
		public void on_description_changed (Entry entry) {
			edited = true;
		}

		[CCode (instance_pos = -1)]
		public bool on_description_focus_out_event (Entry entry, EventFocus ev) {
			if (edited) {
				role.description = entry.get_text ();
				save ();
			}
			return false;
		}
	}
}