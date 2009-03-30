using GLib;
using Gtk;
using Gdk;
using Sqlp;

namespace SqlpGtk {

	public class ModelFields : Fieldset <Model> {

		public TagManager tag_manager {	get; construct;	}

		public ModelFields (TagManager tag_manager) {
			this.gui_name = "model_fields";
			this.tag_manager = tag_manager;
		}

		private Entry name;
		private Entry abbreviation;
		private Entry make;
		private Entry type;

		construct {
			Logbook l; // Vala seems to need a declaration to generate correct dependencies in c headers.

			name = gui.object ("name") as Entry;
			abbreviation = gui.object ("abbreviation") as Entry;
			make = gui.object ("make") as Entry;
			type = gui.object ("type") as Entry;

			tag_manager.add_tagging_button = gui.object ("add_tagging") as Button;
			tag_manager.remove_tagging_button = gui.object ("remove_tagging") as Button;
			tag_manager.add_tag_button = gui.object ("add_tag") as Button;
			tag_manager.remove_tag_button = gui.object ("remove_tag") as Button;
// 			(gui.object ("tags") as ScrolledWindow).add_with_viewport (tag_manager.tags_view);
// 			(gui.object ("taggings") as ScrolledWindow).add_with_viewport (tag_manager.taggings_view);
// 			tag_manager.tags_view.show ();
// 			tag_manager.taggings_view.show ();
			set_slot ("tags", tag_manager.tags_view);
			set_slot ("taggings", tag_manager.taggings_view);
		}

		protected override void set_fields_from_record () {
			name.set_text (record.name);
			abbreviation.set_text (record.abbreviation);
			tag_manager.object_id = record.id;
		}

		protected override void set_record_from_fields () {
		}

		[CCode (instance_pos = -1)]
		public void on_name_changed (Entry entry) {
			edited = true;
		}

		[CCode (instance_pos = -1)]
		public bool on_name_focus_out_event (Entry entry, EventFocus ev) {
			record.name = name.get_text ();
			if (edited && ! save ()) {
				Idle.add ( () => { select_entry (this.name); } );
			}
			return false;
		}

		[CCode (instance_pos = -1)]
		public void on_abbreviation_changed (Entry entry) {
			edited = true;
		}

		[CCode (instance_pos = -1)]
		public bool on_abbreviation_focus_out_event (Entry entry, EventFocus ev) {
			record.abbreviation = abbreviation.get_text ();
			if (edited && ! save ()) {
				Idle.add ( () => { select_entry (this.abbreviation); } );
			}
			return false;
		}

		[CCode (instance_pos = -1)]
		public void on_make_changed (Entry entry) {
			edited = true;
		}

		[CCode (instance_pos = -1)]
		public bool on_make_focus_out_event (Entry entry, EventFocus ev) {
			record.make = make.get_text ();
			if (edited && ! save ()) {
				Idle.add ( () => { select_entry (this.make); } );
			}
			return false;
		}

		[CCode (instance_pos = -1)]
		public void on_type_changed (Entry entry) {
			edited = true;
		}

	}
}
