using GLib;
using Gtk;
using Gdk;
using Sqlp;

namespace SqlpGtk {

	public class ModelFields : Fieldset {

		public Model model {
			get { return this.record as Model; }
			set { this.record = value; }
		}

		public ModelFields (Sqlp.Table table) {
			this.gui_name = "model_fields";
			this.table = table;
		}

		private Entry name;
		private Entry abbreviation;
		private Entry make;
		private Entry type;

		private TagChooser property_chooser;

		construct {
			Logbook l; // Vala seems to need a declaration to generate correct dependencies in c headers.
			var logbook = table.database as Logbook;

			name = gui.object ("name") as Entry;
			abbreviation = gui.object ("abbreviation") as Entry;
			make = gui.object ("make") as Entry;
			type = gui.object ("type") as Entry;

			property_chooser = new TagChooser (table, logbook.model_taggings, logbook.model_tags);			
			set_slot ("property_chooser", property_chooser);
		}

		protected override void set_fields_from_record () {
			name.set_text (model.name);
			abbreviation.set_text (model.abbreviation);
			property_chooser.parent_id = model.id;
		}

		[CCode (instance_pos = -1)]
		public void on_name_changed (Entry entry) {
			edited = true;
		}

		[CCode (instance_pos = -1)]
		public bool on_name_focus_out_event (Entry entry, EventFocus ev) {
			model.name = name.get_text ();
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
			model.abbreviation = abbreviation.get_text ();
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
			model.make = make.get_text ();
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
