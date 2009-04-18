using Gtk;
using Gdk;
using Sqlp;

namespace SqlpGtk {
	public class FlightFields : Fieldset <Flight> {

		private Logbook logbook;

		private Entry date;
		private SpinButton leg;
		private Entry route;
		private ToggleButton icao;
		private ComboBoxEntry role;
		private Entry role_entry;
		private ComboBoxEntry aircraft_registration;
		private Entry aircraft_registration_entry;
		private ComboBoxEntry aircraft_tail;
		private Entry aircraft_tail_entry;
		private ToggleButton tail;
		private Entry duration;
		private Entry actual_out;
		private Entry actual_in;
		private Entry scheduled_duration;
		private Entry scheduled_out;
		private Entry scheduled_in;
		private ToggleButton utc;
		private Entry night;
		private Entry instrument;
		private Entry hood;
		private Entry flight_number;
		private Entry trip;
		private Entry trip_date;

		private GlideEditor glide_editor;
		private ApproachEditor approach_editor;
		private HoldEditor hold_editor;
		private TakeoffEditor takeoff_editor;
		private LandingEditor landing_editor;

		private ToggleButton cross_country;

		private TreeView people_view;
		private Button   person_add;
		private Button   person_remove;
		private Label    person_summary;

		private TreeView crew_view;
		private Button crew_add;
		private Button crew_remove;
		private Label crew_summary;

		private TextView notes;

		private TableObserverStore role_store;
		private TableObserverStore aircraft_store;

		private TagManager tag_manager;

		public FlightFields (Sqlp.Table table) {
			this.gui_name = "flight_fields";
			this.table = table;
		}

		private Entry aircraft_entry {
			get { return tail.active ? aircraft_tail_entry : aircraft_registration_entry; }
		}
		
		construct {
			logbook = table.database;

			glide_editor = new GlideEditor (logbook);
			set_slot ("glides", glide_editor);

			approach_editor = new ApproachEditor (logbook);
			set_slot ("approaches", approach_editor);

			hold_editor = new HoldEditor (logbook);
			set_slot ("holds", hold_editor);

			takeoff_editor = new TakeoffEditor (logbook);
			set_slot ("takeoffs", takeoff_editor);

			landing_editor = new LandingEditor (logbook);
			set_slot ("landings", landing_editor);

			tag_manager = new TagManager (logbook.flight, logbook.flight_taggings, logbook.flight_tags);
			tag_manager.add_tagging_button = gui.object ("add_tagging") as Button;
			tag_manager.remove_tagging_button = gui.object ("remove_tagging") as Button;
			tag_manager.add_tag_button = gui.object ("add_tag") as Button;
			tag_manager.remove_tag_button = gui.object ("remove_tag") as Button;
			set_slot ("tags", tag_manager.tags_view);
			set_slot ("taggings", tag_manager.taggings_view);

			var date_edit = new Entry ();
			gui.box ("date_slot").pack_start_defaults (date_edit);
			date_edit.show ();
			date_edit.changed += (date_edit) => {
				on_date_changed (date_edit);
			};
			date_edit.focus_out_event += (date_edit, event_focus) => {
				on_date_focus_out_event (date_edit, event_focus);
			};
			
			date				= date_edit;
			leg					= gui.object ("leg")					as SpinButton;
			route				= gui.object ("route")					as Entry;
			icao				= gui.object ("icao")					as ToggleButton;
			tail				= gui.object ("tail")					as ToggleButton;
			duration			= gui.object ("duration")				as Entry;
			actual_out			= gui.object ("actual_out")				as Entry;
			actual_in			= gui.object ("actual_in")				as Entry;
			scheduled_duration	= gui.object ("scheduled_duration")		as Entry;
			scheduled_out		= gui.object ("scheduled_out")			as Entry;
			scheduled_in		= gui.object ("scheduled_in")			as Entry;
			utc					= gui.object ("utc")					as ToggleButton;
			night				= gui.object ("night")					as Entry;
			instrument			= gui.object ("instrument")				as Entry;
			hood				= gui.object ("hood")					as Entry;
			flight_number		= gui.object ("flight_number")			as Entry;
			trip				= gui.object ("trip")					as Entry;
			trip_date			= gui.object ("trip_date")				as Entry;


			// Treemodel and combobox for picking a role
			role_store = new TableObserverStore ();
			role_store.select_sql = "SELECT * FROM Roles";
			role_store.database = logbook;
			role_store.observe (logbook.role);
			role = new ComboBoxEntry.with_model (role_store, 1);
			role_entry = role.get_child () as Entry;
			role_entry.changed += on_role_entry_changed;
			role_entry.focus_out_event += on_role_entry_focus_out_event;
			(gui.object ("role_slot") as Box).add (role);
			role.show ();


			// Tree model of aircraft for convenient picking
			aircraft_store = new TableObserverStore ();
			aircraft_store.select_sql = "SELECT * FROM Aircraft";
			aircraft_store.database = logbook;
			aircraft_store.observe (logbook.aircraft);

			// Combobox for aircraft registration.
			aircraft_registration = new ComboBoxEntry.with_model (aircraft_store, 2);
			aircraft_registration_entry = aircraft_registration.get_child () as Entry;
			aircraft_registration_entry.changed += on_aircraft_entry_changed;
			aircraft_registration_entry.focus_out_event += on_aircraft_entry_focus_out_event;
			(gui.object ("aircraft_slot") as Box).add (aircraft_registration);

			// And for aircraft tail.
			aircraft_tail = new ComboBoxEntry.with_model (aircraft_store, 3);
			aircraft_tail_entry = aircraft_tail.get_child () as Entry;
			aircraft_tail_entry.changed += on_aircraft_entry_changed;
			aircraft_tail_entry.focus_out_event += on_aircraft_entry_focus_out_event;
			(gui.object ("aircraft_slot") as Box).add (aircraft_tail);

			set_aircraft_combobox_visibility ();

		}

		private void set_aircraft_combobox_visibility () {
			aircraft_tail.visible = tail.active;
			aircraft_registration.visible = ! tail.active;
		}

		protected override void set_fields_from_record () {
			date.set_text  (record.date.to_iso8601 ());
			leg.set_text (record.leg.to_string ());
			route.set_text (record.route.to_string_icao ());
		   	role_entry.set_text (record.role == null ? "" : record.role.abbreviation);
			aircraft_entry.set_text (record.aircraft == null ? "" : (tail.active ? record.aircraft.tail : record.aircraft.registration));
			duration.set_text (record.duration.to_string ());
			actual_out.set_text (record.actual_out.to_iso8601 ());
			actual_in.set_text (record.actual_in.to_iso8601 ());
			scheduled_duration.set_text (record.scheduled_duration.to_string ());
			scheduled_out.set_text (record.scheduled_out.to_iso8601 ());
			scheduled_in.set_text (record.scheduled_in.to_iso8601 ());
			night.set_text (record.night.to_string ());
			instrument.set_text (record.instrument.to_string ());
			hood.set_text (record.hood.to_string ());
			trip.set_text (empty_if_null(record.trip));
			trip_date.set_text (record.trip_date.to_iso8601 ());
			flight_number.set_text (empty_if_null(record.flight_number));
			glide_editor.parent_id = record.id;
			approach_editor.parent_id = record.id;
			hold_editor.parent_id = record.id;
			takeoff_editor.parent_id = record.id;
			landing_editor.parent_id = record.id;
			tag_manager.object_id = record.id;
		}

		public override void set_record_from_fields () {
			record.date = Sqlp.Date.from_iso8601 (date.get_text ());
			record.leg = leg.get_value_as_int ();
			record.route.read (route.get_text ());
//			record.role = browser.book.logbook.role.find_by_abbreviation (role.get_text ());
		}

		private void reconcile_time_entries (Entry changed, Entry start, Entry stop, Entry elapsed) {
			
		}

		private void format_time_entry_for_edit (Entry entry) {
			string text = entry.get_text ();
			if (! text.validate ()) return;
			bool save_edited = edited;
			entry.set_text (filter_non_ascii_digits (text));
			edited = save_edited;
		}

		private string filter_non_ascii_digits (string text) {
			char* cursor = text;
			var output = new StringBuilder.sized (16);
			while (*cursor != '\0') {
				if (*cursor >= 48 && *cursor <= 57) output.append_c (*cursor);
				cursor = ((string) cursor).next_char ();
			}
			return output.str;
		}

		private void format_time_of_day_entry (Entry entry) {
 			string text = entry.get_text ();
			if (text == "") return;
			if (! text.validate ()) {
				entry.set_text ("");
				return;
			}
			bool save_edited = edited;
			var hm = filter_non_ascii_digits(text).to_int ();
			var minutes = (hm % 100) % 60;
			var hours = (hm / 100) % 24;
			entry.set_text ("%02d:%02d".printf (hours, minutes));
			edited = save_edited;
		}

		private void format_elapsed_time_entry (Entry entry) {
 			string text = entry.get_text ();
			if (text[0] == '\0' || ! text.validate ()) {
				entry.set_text ("");
				return;
			}
			var hm = filter_non_ascii_digits(text).to_int ();
			var minutes = (hm % 100) % 60;
			var hours = (hm / 100);
			bool save_edited = edited;
			entry.set_text ("%d+%02d".printf (hours, minutes));
			edited = save_edited;
		}

		private Timezone timezone_for_departure () {
			return (utc.active || (record.origin == null)) ? Timezone ("UTC") : record.origin.timezone;
		}

		private Timezone timezone_for_arrival () {
			return (utc.active || (record.destination == null)) ? Timezone ("UTC") : record.destination.timezone;
		}

		private Sqlp.Date date_for_difftime () {
			var ds = date.get_text ();
			if (ds[0] != '\0') {
				var d = Sqlp.Date.from_iso8601 (ds);
				if (d.valid ()) return d;
			}
			return Sqlp.Date.from_iso8601 ("1979-12-25");
		}

		[CCode (instance_pos = -1)]
		public void on_date_changed (Entry entry) {
			edited = true;
		}
		
		[CCode (instance_pos = -1)]
		public bool on_date_focus_in_event (Entry entry, EventFocus ev) {
			return false;
		}

		[CCode (instance_pos = -1)]
		public bool on_date_focus_out_event (Entry entry, EventFocus ev) {
			if (edited) {
				set_date ();
				save ();
			}
			return false;
		}

		private void set_date () {
			if (date.get_text ().length == 0) {
				record.date.clear ();
				return;
			}
			var newdate = Sqlp.Date.from_iso8601 (date.get_text ());
			if (newdate.valid ()) {
				record.date = newdate;
				date.set_text (newdate.to_iso8601 ());
			} else {
				Idle.add ( () => { select_entry (this.date); } );
			}
		}

		[CCode (instance_pos = -1)]
		public void on_trip_date_changed (Entry entry) {
			this.edited = true;
		}

		[CCode (instance_pos = -1)]
		public bool on_trip_date_focus_in_event (Entry entry, EventFocus ev) {
			return false;
		}

		[CCode (instance_pos = -1)]
		public bool on_trip_date_focus_out_event (Entry entry, EventFocus ev) {
			if (edited) {
				set_trip_date ();
				save ();
			}
			return false;
		}

		private void set_trip_date () {
			if (trip_date.get_text ().length == 0) {
				record.trip_date.clear ();
				return;
			}
			var newdate = Sqlp.Date.from_iso8601 (trip_date.get_text ());
			if (newdate.valid ()) {
				record.trip_date = newdate;
				trip_date.set_text (newdate.to_iso8601 ());
			} else {
				Idle.add ( () => { select_entry (this.trip_date); } );
			}
		}

		[CCode (instance_pos = -1)]
		public void on_role_changed (ComboBoxEntry entry) {
			this.edited = true;
		}

		[CCode (instance_pos = -1)]
		public bool on_role_focus_out_event (Entry entry, EventFocus ev)
		{
			message ("on_role_focus_oute_event ()");
			return false;
		}

		public void on_role_entry_changed (Entry entry) {
			edited = true;
		}

		public bool on_role_entry_focus_out_event (Entry entry, EventFocus ev) {
			if (edited) {
				record.role = record.table.database.role.find_or_new_by_abbreviation (role_entry.get_text ());
				save ();
			}
			return false;
		}

		[CCode (instance_pos = -1)]
		public void on_trip_changed (Entry entry)
		{
			
		}

		[CCode (instance_pos = -1)]
		public void on_flight_number_changed (Entry entry)
		{
		}

		[CCode (instance_pos = -1)]
		public void on_leg_value_changed (SpinButton button)
		{
		}

		[CCode (instance_pos = -1)]
		public void on_aircraft_changed (ComboBoxEntry entry)
		{
		}

		[CCode (instance_pos = -1)]
		public bool on_aircraft_focus_out_event (ComboBoxEntry entry, EventFocus ev)
		{
			return false;
		}

		public void on_aircraft_entry_changed (Entry entry) {
			edited = true;
		}

		public bool on_aircraft_entry_focus_out_event (Entry entry, EventFocus ev) {
			if (edited) {
				var aircraft_table = record.table.database.aircraft;
				record.aircraft = tail.active ?
					aircraft_table.find_or_new_by_tail (aircraft_entry.get_text ()) :
					aircraft_table.find_or_new_by_registration (aircraft_entry.get_text ());
				save ();
			}
			return false;
		}

		[CCode (instance_pos = -1)]
		public void on_route_changed (Entry entry)
		{
			edited = true;
		}

		[CCode (instance_pos = -1)]
		public bool on_route_focus_out_event (Entry entry, EventFocus ev)
		{
			var origin = record.origin != null ? record.origin.id : 0;
			var destination = record.destination != null ? record.destination.id : 0;
			record.read_full_route (route.get_text ());
			route.set_text (record.show_full_route_icao ());
			if (origin != record.origin.id) {
				// origin changed
				message ("origin changed");
			}
			if (destination != record.destination.id) {
				// destination changed
				message ("destination changed");
			}
			return false;
		}

		[CCode (instance_pos = -1)]
		public bool on_scheduled_out_focus_in_event (Entry entry, EventFocus ev)
		{
			format_time_entry_for_edit (entry);
			return false;
		}

		[CCode (instance_pos = -1)]
		public bool on_scheduled_out_focus_out_event (Entry entry, EventFocus ev)
		{
			format_time_of_day_entry (entry);
			record.scheduled_out = TimeOfDay.from_timezone_time (timezone_for_departure (), entry.get_text ());
			if (! record.scheduled_out.valid ()) return false;
			if (record.scheduled_in.valid ()) {
				record.scheduled_duration = Datetime (date_for_difftime (), record.scheduled_out).duration_to_time_of_day (record.scheduled_in);
				scheduled_duration.set_text (record.scheduled_duration.to_string ());
			} else if (record.scheduled_duration.valid ()) {
				var tmp = Datetime (date_for_difftime (), record.scheduled_out).add (record.scheduled_duration);
				record.scheduled_in = tmp.time_of_day;
				scheduled_in.set_text (record.scheduled_in.to_iso8601 ());
			}
			if (edited) {
				save ();
			}
			return false;
		}

		[CCode (instance_pos = -1)]
		public void on_scheduled_out_changed (Entry entry)
		{
			edited = true;
		}

		[CCode (instance_pos = -1)]
		public bool on_scheduled_in_focus_in_event (Entry entry, EventFocus ev)
		{
			format_time_entry_for_edit (entry);
			record.scheduled_in = TimeOfDay.from_timezone_time (timezone_for_arrival (), entry.get_text ());
			return false;
		}

		[CCode (instance_pos = -1)]
		public bool on_scheduled_in_focus_out_event (Entry entry, EventFocus ev)
		{
 			format_time_of_day_entry (entry);
			record.scheduled_in = TimeOfDay.from_timezone_time (timezone_for_arrival (), entry.get_text ());
			if (! record.scheduled_in.valid ()) return false;
			if (record.scheduled_out.valid ()) {
				record.scheduled_duration = Datetime (date_for_difftime (), record.scheduled_out).duration_to_time_of_day (record.scheduled_in);
				scheduled_duration.set_text (record.scheduled_duration.to_string ());
			} else if (record.scheduled_duration.valid ()) {
				var tmp = Datetime (date_for_difftime (), record.scheduled_in).subtract (record.scheduled_duration);
				record.scheduled_out = tmp.time_of_day;
				scheduled_out.set_text (record.scheduled_out.to_iso8601 ());
			}
			if (edited) {
				save ();
			}
			return false;
		}

		[CCode (instance_pos = -1)]
		public void on_scheduled_in_changed (Entry entry)
		{
			edited = true;
		}

		[CCode (instance_pos = -1)]
		public bool on_scheduled_duration_focus_in_event (Entry entry, EventFocus ev)
		{
			format_time_entry_for_edit (entry);
			return false;
		}

		[CCode (instance_pos = -1)]
		public bool on_scheduled_duration_focus_out_event (Entry entry, EventFocus ev)
		{
			format_elapsed_time_entry (entry);
			record.scheduled_duration = Duration.from_string (entry.get_text ());
			if (! record.scheduled_duration.valid ()) {
				return false;
			}
			if (record.scheduled_out.valid ()) {
				var tmp = Datetime (date_for_difftime (), record.scheduled_out).add (record.scheduled_duration);
				record.scheduled_in = tmp.time_of_day;
				scheduled_in.set_text (record.scheduled_in.to_iso8601 ());
			} else if (record.scheduled_in.valid ()) {
				var tmp = Datetime (date_for_difftime (), record.scheduled_in).subtract (record.scheduled_duration);
				record.scheduled_out = tmp.time_of_day;
				scheduled_out.set_text (record.scheduled_out.to_iso8601 ());
			}
			if (edited) {
				save ();
			}
			return false;
		}

		[CCode (instance_pos = -1)]
		public void on_scheduled_duration_changed (Entry entry)
		{
			edited = true;
		}

		[CCode (instance_pos = -1)]
		public bool on_actual_out_focus_in_event (Entry entry, EventFocus ev)
		{
			format_time_entry_for_edit (entry);
			return false;
		}

		[CCode (instance_pos = -1)]
		public bool on_actual_out_focus_out_event (Entry entry, EventFocus ev)
		{
			format_time_of_day_entry (entry);
			reconcile_time_entries (entry, actual_out, actual_in, duration);
			return false;
		}

		[CCode (instance_pos = -1)]
		public void on_actual_out_changed (Entry entry)
		{
			edited = true;
		}

		[CCode (instance_pos = -1)]
		public bool on_actual_in_focus_in_event (Entry entry, EventFocus ev)
		{
			format_time_entry_for_edit (entry);
			return false;
		}

		[CCode (instance_pos = -1)]
		public bool on_actual_in_focus_out_event (Entry entry, EventFocus ev)
		{
			format_time_of_day_entry (entry);
			reconcile_time_entries (entry, actual_out, actual_in, duration);
			return false;
		}

		[CCode (instance_pos = -1)]
		public void on_actual_in_changed (Entry entry)
		{
			edited = true;
		}

		[CCode (instance_pos = -1)]
		public bool on_duration_focus_in_event (Entry entry, EventFocus ev)
		{
			format_time_entry_for_edit (entry);
			return false;
		}

		[CCode (instance_pos = -1)]
		public bool on_duration_focus_out_event (Entry entry, EventFocus ev)
		{
			format_elapsed_time_entry (entry);
			reconcile_time_entries (entry, actual_out, actual_in, duration);
			return false;
		}

		[CCode (instance_pos = -1)]
		public void on_duration_changed (Entry entry)
		{
			edited = true;
		}

		[CCode (instance_pos = -1)]
		public bool on_instrument_focus_in_event (Entry entry, EventFocus ev)
		{
			format_time_entry_for_edit (entry);
			return false;
		}

		[CCode (instance_pos = -1)]
		public bool on_instrument_focus_out_event (Entry entry, EventFocus ev)
		{
			format_elapsed_time_entry (entry);
			return false;
		}

		[CCode (instance_pos = -1)]
		public void on_instrument_changed (Entry entry)
		{
			edited = true;
		}

		[CCode (instance_pos = -1)]
		public bool on_hood_focus_in_event (Entry entry, EventFocus ev)
		{
			format_time_entry_for_edit (entry);
			return false;
		}

		[CCode (instance_pos = -1)]
		public bool on_hood_focus_out_event (Entry entry, EventFocus ev)
		{
			format_elapsed_time_entry (entry);
			return false;
		}

		[CCode (instance_pos = -1)]
		public void on_hood_changed (Entry entry)
		{
			edited = true;
		}

		[CCode (instance_pos = -1)]
		public bool on_night_focus_in_event (Entry entry, EventFocus ev)
		{
			format_time_entry_for_edit (entry);
			return false;
		}

		[CCode (instance_pos = -1)]
		public bool on_night_focus_out_event (Entry entry, EventFocus ev)
		{
			format_elapsed_time_entry (entry);
			return false;
		}

		[CCode (instance_pos = -1)]
		public void on_night_changed (Entry entry)
		{
			edited = true;
		}

 		[CCode (instance_pos = -1)]
		public void on_utc_toggled (ToggleButton button)
		{
			Timezone origin_tz = Timezone ("UTC");
			Timezone destination_tz = Timezone ("UTC");
			if (! button.active) {
				origin_tz = timezone_for_departure ();
				destination_tz = timezone_for_arrival ();
			}
// 			record.scheduled_out.move_to_timezone (origin_tz);
// 			record.scheduled_in.move_to_timezone (destination_tz);
// 			record.actual_out.move_to_timezone (origin_tz);
// 			record.actual_in.move_to_timezone (destination_tz);
			var save_edited = edited;
			scheduled_out.set_text (record.scheduled_out.to_iso8601 ());
			scheduled_in.set_text (record.scheduled_in.to_iso8601 ());
			actual_out.set_text (record.actual_out.to_iso8601 ());
			actual_in.set_text (record.actual_in.to_iso8601 ());
			edited = save_edited;
		}

		[CCode (instance_pos = -1)]
		public void on_icao_toggled (ToggleButton button)
		{
		}

		[CCode (instance_pos = -1)]
		public void on_tail_toggled(ToggleButton button) {
			set_aircraft_combobox_visibility ();
		}


		public void on_person_add_clicked (Button button) {
		}

		public void on_person_remove_clicked (Button button) {
		}

		public void on_crew_add_clicked (Button button) {
		}

		public void on_crew_remove_clicked (Button button) {
		}

		[CCode (instance_pos = -1)]
		public void on_notes_changed(TextBuffer tb)
		{
			edited = true;
		}

		[CCode (instance_pos = -1)]
		public void on_cross_country_toggled(Widget button)
		{
		}
	}
}