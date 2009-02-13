using GLib;
using Gtk;
using Gdk;

namespace SqlPilotGtk {
	namespace CbFlightFieldsAirline {
		public static void on_date_changed (Entry entry, Pane fields) {
			assert(fields is Pane);
		}

		public static bool on_date_focus_in_event (Entry entry, EventFocus ev, Pane fields) {
			return false;
		}

		public static bool on_date_focus_out_event (Entry entry, EventFocus ev, Pane fields) {
			return false;
		}

		public static void on_tripdate_changed (Entry entry, Pane fields) {
			assert(fields is Pane);
		}

		public static bool on_tripdate_focus_in_event (Entry entry, EventFocus ev, Pane fields) {
			return false;
		}

		public static bool on_tripdate_focus_out_event (Entry entry, EventFocus ev, Pane fields) {
			return false;
		}

		public static void on_role_changed (Entry entry, Pane fields) {
		}

		public static void on_trip_changed(Entry entry, Pane fields)
		{
		}

		public static void on_leg_value_changed(Widget button, Pane fields)
		{
		}

		public static bool on_role_focus_out_event(Entry entry, EventFocus ev, Pane fields)
		{
			//flights_refresh_role_utilized(logbook);
			return false;
		}

// 		public static void on_fltno_changed(Entry entry, Pane fields)
// 		{
// 		}

// 		public static void on_tail_changed(Entry entry, Pane fields)
// 		{
// 		}

// 		public static void on_fleetno_changed(Entry entry, Pane fields)
// 		{
// 		}

// 		public static bool on_tail_focus_out_event(Entry entry, EventFocus ev, Pane fields)
// 		{
// 		}

// 		public static bool on_fleetno_focus_out_event(Entry entry, EventFocus ev, Pane fields)
// 		{
// 		}

// 		public static void on_depicao_changed(Entry entry, Pane fields)
// 		{
// 		}

// 		public static bool on_depicao_focus_out_event(Entry entry, EventFocus ev, Pane fields)
// 		{
// 		}

// 		public static void on_depiata_changed(Entry entry, Pane fields)
// 		{
// 		}

// 		public static bool on_depiata_focus_out_event(Entry entry, EventFocus ev, Pane fields)
// 		{
// 		}

// 		public static void on_arricao_changed(Entry entry, Pane fields)
// 		{
// 		}

// 		public static bool on_arricao_focus_out_event(Entry entry, EventFocus ev, Pane fields)
// 		{
// 		}

// 		public static void on_arriata_changed(Entry entry, Pane fields)
// 		{
// 		}

// 		public static bool on_arriata_focus_out_event(Entry entry, EventFocus ev, Pane fields)
// 		{
// 		}

// 		public static bool on_sout_focus_in_event(Entry entry, EventFocus ev, Pane fields)
// 		{
// 		}
// 		public static bool on_sout_focus_out_event(Entry entry, EventFocus ev, Pane fields)
// 		{
// 		}
// 		public static void on_sout_changed(Entry entry, Pane fields)
// 		{
// 		}

// 		public static bool on_sin_focus_in_event(Entry entry, EventFocus ev, Pane fields)
// 		{
// 		}
// 		public static bool on_sin_focus_out_event(Entry entry, EventFocus ev, Pane fields)
// 		{
// 		}
// 		public static void on_sin_changed(Entry entry, Pane fields)
// 		{
// 		}

// 		public static bool on_sdur_focus_in_event(Entry entry, EventFocus ev, Pane fields)
// 		{
// 		}
// 		public static bool on_sdur_focus_out_event(Entry entry, EventFocus ev, Pane fields)
// 		{
// 		}

// 		public static void on_sdur_changed(Entry entry, Pane fields)
// 		{
// 		}

// 		public static bool on_aout_focus_in_event(Entry entry, EventFocus ev, Pane fields)
// 		{
// 		}
// 		public static bool on_aout_focus_out_event(Entry entry, EventFocus ev, Pane fields)
// 		{
// 		}
// 		public static void on_aout_changed(Entry entry, Pane fields)
// 		{
// 		}

// 		public static bool on_ain_focus_in_event(Entry entry, EventFocus ev, Pane fields)
// 		{
// 		}

// 		public static bool on_ain_focus_out_event(Entry entry, EventFocus ev, Pane fields)
// 		{
// 		}

// 		public static void on_ain_changed(Entry entry, Pane fields)
// 		{
// 		}

// 		public static bool on_dur_focus_in_event(Entry entry, EventFocus ev, Pane fields)
// 		{
// 		}

// 		public static bool on_dur_focus_out_event(Entry entry, EventFocus ev, Pane fields)
// 		{
// 		}

// 		public static void on_dur_changed(Entry entry, Pane fields)
// 		{
// 		}

// 		public static bool on_inst_focus_in_event(Entry entry, EventFocus ev, Pane fields)
// 		{
// 		}

// 		public static bool on_inst_focus_out_event(Entry entry, EventFocus ev, Pane fields)
// 		{
// 		}

// 		public static void on_inst_changed(Entry entry, Pane fields)
// 		{
// 		}

// 		public static bool on_siminst_focus_in_event(Entry entry, EventFocus ev, Pane fields)
// 		{
// 		}

// 		public static bool on_siminst_focus_out_event(Entry entry, EventFocus ev, Pane fields)
// 		{
// 		}

// 		public static void on_siminst_changed(Entry entry, Pane fields)
// 		{
// 		}

// 		public static bool on_night_focus_in_event(Entry entry, EventFocus ev, Pane fields)
// 		{
// 		}

// 		public static bool on_night_focus_out_event(Entry entry, EventFocus ev, Pane fields)
// 		{
// 		}

// 		public static void on_night_changed(Entry entry, Pane fields)
// 		{
// 		}

// 		public static void on_selection_changed(GtkTreeSelection *selection, Pane fields)
// 		{
// 		}

// 		public static void on_utc_toggled(GtkWidget *button, Logbook *logb)
// 		{
// 		}

// 		public static void on_icao_toggle_toggled(GtkWidget *button, Pane fields)
// 		{
// 		}

// 		public static void on_fleetno_toggle_toggled(GtkToggleButton *button, Pane fields)
// 		{
// 		}

// 		public static void on_crew_changed(GtkTextBuffer *tb, Pane fields)
// 		{
// 		}

// 		public static void on_notes_changed(GtkTextBuffer *tb, Pane fields)
// 		{
// 		}

// 		public static void on_aprch_changed(GtkWidget *entry, Pane fields)
// 		{
// 		}

// 		public static void on_xc_toggled(GtkWidget *button, Pane fields)
// 		{
// 		}

// 		public static void on_hold_toggled(GtkWidget *button, Pane fields)
// 		{
// 		}

// 		public static void on_dland_value_changed(GtkWidget *button, Pane fields)
// 		{
// 		}

// 		public static void on_nland_value_changed(GtkWidget *button, Pane fields)
// 		{
// 		}

// 		public static void on_armdel_btn_toggled(GtkToggleButton *button, Pane fields)
// 		{
// 		}

// 		public static void on_del_btn_clicked(GtkButton *button, Pane fields)
// 		{
// 		}

// 		public static void on_new_btn_clicked(GtkButton *button, Pane fields)
// 		{
// 		}

// 		public static void on_save_btn_clicked(GtkButton *button, Pane fields)
// 		{
// 		}

// 		public static void on_routeicao_changed(Entry entry, Pane fields)
// 		{
// 		}

// 		gboolean on_routeicao_focus_out_event(Entry entry, GdkEventFocus *f, Pane fields)
// 		{
// 		}

// 		public static void on_routeiata_changed(Entry entry, Pane fields)
// 		{
// 		}

// 		gboolean on_routeiata_focus_out_event(Entry entry, GdkEventFocus *f, Pane fields)
// 		{

// 		}
	}
}