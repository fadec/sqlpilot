using Gtk;
using Gdk;
using Sqlp;

namespace SqlpGtk {

	public class RoleFields : Fieldset <Role> {

		private TagChooser tag_chooser;

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

		public RoleFields (Sqlp.Table table) {
			this.gui_name = "role_fields";
			this.table = table;
		}

		construct {
			var logbook = table.database;

			abbreviation	= gui.object ("abbreviation")			 as Entry;
			description		= gui.object ("description")			 as Entry;
			tag_chooser = new TagChooser (table, logbook.role_taggings, logbook.role_tags);			
			set_slot ("properties", tag_chooser);
		}

		protected override void set_fields_from_record () {
			assert (record is Record);
 			abbreviation.set_text (record.abbreviation != null ? record.abbreviation : "");
			description.set_text (record.description != null ? record.description : "");
			tag_chooser.object_id = record.id;
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
		public void on_description_changed (Entry entry) {
			edited = true;
		}

		[CCode (instance_pos = -1)]
		public bool on_description_focus_out_event (Entry entry, EventFocus ev) {
			if (edited) {
				record.description = entry.get_text ();
				save ();
			}
			return false;
		}
	}
}