using GLib;
using Gtk;
using Gdk;

namespace SqlpGtk {
	public class AirlineFlightFields : Pane {

		public AirlineFlightFields () {
			base ("airline_flight_fields");
		}

		[CCode (instance_pos = -1)]
		public void on_date_changed (Entry entry) {
			message ("Yay\n");
		}
		
		[CCode (instance_pos = -1)]
		public bool on_date_focus_in_event (Entry entry, EventFocus ev) {
			return false;
		}

		[CCode (instance_pos = -1)]
		public bool on_date_focus_out_event (Entry entry, EventFocus ev) {
			return false;
		}

		[CCode (instance_pos = -1)]
		public void on_tripdate_changed (Entry entry) {
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