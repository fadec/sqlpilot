using Gnome;
using Gtk;
using Gdk;
using Sqlp;

namespace SqlpGtk {
	public class FlightFields : Fieldset <Flight> {

		private Entry date;
		private SpinButton leg;
		private Entry route;
		private Entry role;
		private Entry aircraft;
		private Entry duration;
		private Entry actual_out;
		private Entry actual_in;
		private Entry scheduled_duration;
		private Entry scheduled_out;
		private Entry scheduled_in;
		private Entry night;
		private Entry instrument;
		private Entry hood;
		private Entry flight_number;
		private Entry trip;
		private Entry trip_date;

		public FlightFields () {
			this.gui_name = "flight_fields";
		}

		construct {
			var date_edit = new Entry ();
			gui.box ("date_slot").pack_start_defaults (date_edit);
			date_edit.show ();
			date_edit.changed += (date_edit) => {
				on_date_changed (date_edit);
			};
			date_edit.focus_out_event += (date_edit, event_focus) => {
				on_date_focus_out_event (date_edit, event_focus);
			};
			
			date = date_edit;
			leg = gui.object ("leg") as SpinButton;
			route = gui.object ("route") as Entry;
			role = gui.object ("role") as Entry;
			aircraft = gui.object ("aircraft") as Entry;
			duration = gui.object ("duration") as Entry;
			actual_out = gui.object ("actual_out") as Entry;
			actual_in = gui.object ("actual_in") as Entry;
			scheduled_duration = gui.object ("scheduled_duration") as Entry;
			scheduled_out = gui.object ("scheduled_out") as Entry;
			scheduled_in = gui.object ("scheduled_in") as Entry;
			night = gui.object ("night") as Entry;
			instrument = gui.object ("instrument") as Entry;
			hood = gui.object ("hood") as Entry;
			flight_number = gui.object ("flight_number") as Entry;
			trip = gui.object ("trip") as Entry;
			trip_date = gui.object ("trip_date") as Entry;
		}

		protected override void set_fields_from_record () {
			date.set_text  (record.date.to_iso8601 ());
			leg.set_text (record.leg.to_string ());
			route.set_text (record.route.to_string_icao ());
			role.set_text (record.role == null ? "" : record.role.abbreviation);
			aircraft.set_text (record.aircraft == null ? "" : record.aircraft.tail);
			duration.set_text (record.duration.to_string ());
			actual_out.set_text (record.actual_out.to_iso8601 ());
			actual_in.set_text (record.actual_in.to_iso8601 ());
			scheduled_duration.set_text (record.scheduled_duration.to_string ());
			scheduled_out.set_text (record.scheduled_out.to_iso8601 ());
			scheduled_in.set_text (record.scheduled_in.to_iso8601 ());
			night.set_text (record.night.to_string ());
			instrument.set_text (record.instrument.to_string ());
			hood.set_text (record.hood.to_string ());
			trip.set_text (record.trip);
			trip_date.set_text (record.trip_date.to_iso8601 ());
			flight_number.set_text (record.flight_number);
		}

		public override void set_record_from_fields () {
			record.date = Sqlp.Date.from_iso8601 (date.get_text ());
			record.leg = leg.get_value_as_int ();
			record.route.read (route.get_text ());
//			record.role = browser.book.logbook.role.find_by_abbreviation (role.get_text ());
		}

		[CCode (instance_pos = -1)]
		public void on_date_changed (Entry entry) {
//			this.modified = true;
		}
		
		[CCode (instance_pos = -1)]
		public bool on_date_focus_in_event (Entry entry, EventFocus ev) {
			return false;
		}

		[CCode (instance_pos = -1)]
		public bool on_date_focus_out_event (Entry entry, EventFocus ev) {
			set_date ();
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
				Idle.add (select_date);
			}
		}

		private bool select_date () {
			date.grab_focus ();
			date.select_region (0, -1);
			return false;
		}

		[CCode (instance_pos = -1)]
		public void on_tripdate_changed (Entry entry) {
//			this.modified = true;
		}

		[CCode (instance_pos = -1)]
		public bool on_tripdate_focus_in_event (Entry entry, EventFocus ev) {
			return false;
		}

		[CCode (instance_pos = -1)]
		public bool on_tripdate_focus_out_event (Entry entry, EventFocus ev) {
			return false;
		}

		[CCode (instance_pos = -1)]
		public void on_role_changed (Entry entry) {
//			this.modified = true;
			
		}

		[CCode (instance_pos = -1)]
		public void on_trip_changed(Entry entry)
		{
		}

		[CCode (instance_pos = -1)]
		public void on_leg_value_changed(Widget button)
		{
		}

		[CCode (instance_pos = -1)]
		public bool on_role_focus_out_event(Entry entry, EventFocus ev)
		{
			//flights_refresh_role_utilized(logbook);
			return false;
		}

		[CCode (instance_pos = -1)]
		public void on_fltno_changed(Entry entry)
		{
		}

		[CCode (instance_pos = -1)]
		public void on_aircraft_changed(Entry entry)
		{
		}

		[CCode (instance_pos = -1)]
		public bool on_aircraft_focus_out_event(Entry entry, EventFocus ev)
		{
			return false;
		}

		[CCode (instance_pos = -1)]
		public void on_dep_changed(Entry entry)
		{
		}

		[CCode (instance_pos = -1)]
		public bool on_dep_focus_out_event(Entry entry, EventFocus ev)
		{
			return false;
		}

		[CCode (instance_pos = -1)]
		public void on_arr_changed(Entry entry)
		{
		}

		[CCode (instance_pos = -1)]
		public bool on_arr_focus_out_event(Entry entry, EventFocus ev)
		{
			return false;
		}

		[CCode (instance_pos = -1)]
		public bool on_sout_focus_in_event(Entry entry, EventFocus ev)
		{
			return false;
		}

		[CCode (instance_pos = -1)]
		public bool on_sout_focus_out_event(Entry entry, EventFocus ev)
		{
			return false;
		}

		[CCode (instance_pos = -1)]
		public void on_sout_changed(Entry entry)
		{
		}

		[CCode (instance_pos = -1)]
		public bool on_sin_focus_in_event(Entry entry, EventFocus ev)
		{
			return false;
		}

		[CCode (instance_pos = -1)]
		public bool on_sin_focus_out_event(Entry entry, EventFocus ev)
		{
			return false;
		}

		[CCode (instance_pos = -1)]
		public void on_sin_changed(Entry entry)
		{
		}

		[CCode (instance_pos = -1)]
		public bool on_sdur_focus_in_event(Entry entry, EventFocus ev)
		{
			return false;
		}

		[CCode (instance_pos = -1)]
		public bool on_sdur_focus_out_event(Entry entry, EventFocus ev)
		{
			return false;
		}

		[CCode (instance_pos = -1)]
		public void on_sdur_changed(Entry entry)
		{
		}

		[CCode (instance_pos = -1)]
		public bool on_aout_focus_in_event(Entry entry, EventFocus ev)
		{
			return false;
		}

		[CCode (instance_pos = -1)]
		public bool on_aout_focus_out_event(Entry entry, EventFocus ev)
		{
			return false;
		}

		[CCode (instance_pos = -1)]
		public void on_aout_changed(Entry entry)
		{
		}

		[CCode (instance_pos = -1)]
		public bool on_ain_focus_in_event(Entry entry, EventFocus ev)
		{
			return false;
		}

		[CCode (instance_pos = -1)]
		public bool on_ain_focus_out_event(Entry entry, EventFocus ev)
		{
			return false;
		}

		[CCode (instance_pos = -1)]
		public void on_ain_changed(Entry entry)
		{
		}

		[CCode (instance_pos = -1)]
		public bool on_dur_focus_in_event(Entry entry, EventFocus ev)
		{
			return false;
		}

		[CCode (instance_pos = -1)]
		public bool on_dur_focus_out_event(Entry entry, EventFocus ev)
		{
			return false;
		}

		[CCode (instance_pos = -1)]
		public void on_dur_changed(Entry entry)
		{
		}

		[CCode (instance_pos = -1)]
		public bool on_inst_focus_in_event(Entry entry, EventFocus ev)
		{
			return false;
		}

		[CCode (instance_pos = -1)]
		public bool on_inst_focus_out_event(Entry entry, EventFocus ev)
		{
			return false;
		}

		[CCode (instance_pos = -1)]
		public void on_inst_changed(Entry entry)
		{
		}

		[CCode (instance_pos = -1)]
		public bool on_siminst_focus_in_event(Entry entry, EventFocus ev)
		{
			return false;
		}

		[CCode (instance_pos = -1)]
		public bool on_siminst_focus_out_event(Entry entry, EventFocus ev)
		{
			return false;
		}

		[CCode (instance_pos = -1)]
		public void on_siminst_changed(Entry entry)
		{
		}

		[CCode (instance_pos = -1)]
		public bool on_night_focus_in_event(Entry entry, EventFocus ev)
		{
			return false;
		}

		[CCode (instance_pos = -1)]
		public bool on_night_focus_out_event(Entry entry, EventFocus ev)
		{
			return false;
		}

		[CCode (instance_pos = -1)]
		public void on_night_changed(Entry entry)
		{
		}

		[CCode (instance_pos = -1)]
		public void on_selection_changed(TreeSelection selection)
		{
		}

		[CCode (instance_pos = -1)]
		public void on_utc_toggled(Widget button)
		{
		}

		[CCode (instance_pos = -1)]
		public void on_icao_toggle_toggled(Widget button)
		{
		}

		[CCode (instance_pos = -1)]
		public void on_fleetno_toggle_toggled(ToggleButton button)
		{
		}

		[CCode (instance_pos = -1)]
		public void on_crew_changed(TextBuffer tb)
		{
		}

		[CCode (instance_pos = -1)]
		public void on_notes_changed(TextBuffer tb)
		{
		}

		[CCode (instance_pos = -1)]
		public void on_aprch_changed(Widget entry)
		{
		}

		[CCode (instance_pos = -1)]
		public void on_xc_toggled(Widget button)
		{
		}

		[CCode (instance_pos = -1)]
		public void on_hold_toggled(Widget button)
		{
		}

		[CCode (instance_pos = -1)]
		public void on_dland_value_changed(Widget button)
		{
		}

		[CCode (instance_pos = -1)]
		public void on_nland_value_changed(Widget button)
		{
		}
	}
}