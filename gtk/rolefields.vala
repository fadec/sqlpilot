using Gtk;
using Gdk;
using Sqlp;

namespace SqlpGtk {

	public class RoleFields : Fieldset <Role> {

		public TagManager tag_manager {	get; construct;	}

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

		public RoleFields (Sqlp.Table table, TagManager tag_manager) {
			this.gui_name = "role_fields";
			this.tag_manager = tag_manager;
			this.table = table;
		}

		construct {
			abbreviation	= gui.object ("abbreviation")			 as Entry;
			description		= gui.object ("description")			 as Entry;

			tag_manager.add_tagging_button = gui.object ("add_tagging") as Button;
			tag_manager.remove_tagging_button = gui.object ("remove_tagging") as Button;
			tag_manager.add_tag_button = gui.object ("add_tag") as Button;
			tag_manager.remove_tag_button = gui.object ("remove_tag") as Button;
			set_slot ("tags", tag_manager.tags_view);
			set_slot ("taggings", tag_manager.taggings_view);
		}

		protected override void set_fields_from_record () {
			assert (record is Record);
 			abbreviation.set_text (record.abbreviation != null ? record.abbreviation : "");
			description.set_text (record.description != null ? record.description : "");
			tag_manager.object_id = record.id;
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