using GLib;
using CSV;
using Sqlp; // for main

namespace Sqlp {
	public class Importer {
		private Logbook logbook;

		public Importer () {
		}

		public void do_import ( Logbook logb, FileStream input ) {
			logbook = logb;

			var row = new RowIterator ( input );
			Flight?   flight;
			Role?     role;
			Aircraft? aircraft;
			Model?    model;
			Airport?  dep = null;
			Airport?  arr = null;
// 			while (row.next ()) {
			
// 				var date        = Date.from_iso8601 (row.colstr ("date"));
// 				var fltno       = row.colstr ("fltno");
// 				var dep_ident   = row.colstr ("dep");
// 				var arr_ident   = row.colstr ("arr");
// 				var fleetno     = row.colstr ("fleetno");
// 				var model_ident = row.colstr ("model");
// 				var role_ident  = row.colstr ("role");

// 				flight = logbook.flight.find_by_date_fltno_dep_arr (date.to_iso8601 (),
// 																	fltno,
// 																	dep_ident,
// 																	arr_ident);
// 				if (flight == null) {
// 					stderr.printf("new\n");
// 					flight       = logbook.flight.beget ();
// 					flight.date  = date;
// 					flight.flight_number = flight_number;
// 				} else stderr.printf("update flight_id = %d\n", (int)flight.id);

// 				if (fleetno != "") {
// 					if ((aircraft =
// 						 logbook.aircraft.find_by_fleetno (fleetno)) == null) {
// 						aircraft = logbook.aircraft.beget ();
// 						aircraft.fleetno = fleetno;
// 					}
// 					flight.aircraft = aircraft;
// 					if (model_ident != "") {
// 						if ((model =
// 							 logbook.model.find_by_ident (model_ident)) == null) {
// 							model = logbook.model.beget ();
// 							model.ident = model_ident;
// 						}
// 						aircraft.model = model;
// 					}
// 				}
				
// 				if (role_ident != "") {
// 					if ((role =
// 						 logbook.role.find_by_ident (role_ident)) == null) {
// 						role = logbook.role.beget ();
// 						role.ident = role_ident;
// 					}
// 					flight.role	= role;
// 				}

// 				if (dep_ident != "") {
// 					if ((dep =
// 						 logbook.airport.find_by_ident (dep_ident)) == null) {
// 						dep = logbook.airport.beget ();
// 						dep.set_ident (dep_ident);
// 					}
// 					flight.dep = dep;
// 				}
				
// 				if (arr_ident != "") {
// 					if ((arr =
// 						 logbook.airport.find_by_ident (arr_ident)) == null) {
// 						arr = logbook.airport.beget ();
// 						arr.set_ident (arr_ident);
// 					}
// 					flight.arr = arr;
// 				}

// 				flight.leg				= row.colstr ("leg").to_int ();

// 				var aout				= TimeOfDay.from_timezone_time (dep.timezone, row.colstr ("aout"));
// 				var ain					= TimeOfDay.from_timezone_time (arr.timezone, row.colstr ("ain"));
// 				var aout_datetime		= Datetime (date, aout);
// 				var ain_datetime		= aout_datetime.next_datetime_for_time_of_day (ain);

// 				var sout				= TimeOfDay.from_timezone_time (dep.timezone, row.colstr ("sout"));
// 				var sin					= TimeOfDay.from_timezone_time (arr.timezone, row.colstr ("sin"));
// 				var sout_datetime		= Datetime (date, sout);
// 				var sin_datetime		= sout_datetime.next_datetime_for_time_of_day (sin);

// 				flight.aout				= aout;
// 				flight.ain				= ain;
// 				flight.sout				= sout;
// 				flight.sin				= sin;

// 				var dur_str = row.colstr ("dur");
// 				if (false && dur_str != "") {
// 					flight.dur = Duration.from_string (dur_str);
// 				} else {
// 					flight.dur = ain_datetime.duration (aout_datetime);
// 				}
// 				var sdur_str = row.colstr ("sdur");
// 				if (false && sdur_str != "") {
// 					flight.sdur = Duration.from_string (sdur_str);
// 				} else {
// 					flight.sdur = sin_datetime.duration (sout_datetime);
// 				}

// 				flight.night			= Duration.from_string (row.colstr ("night"));
// 				flight.inst				= Duration.from_string (row.colstr ("inst"));
// 				flight.sim_inst			= Duration.from_string (row.colstr ("siminst"));
// 				flight.aprch			= row.colstr ("aprch");
// 				flight.xc				= str_to_bool (row.colstr ("xc"));
// 				flight.hold				= str_to_bool (row.colstr ("hold"));
// 				flight.dland			= row.colstr ("dland").to_int ();
// 				flight.nland			= row.colstr ("nland").to_int ();
// 				flight.notes			= row.colstr ("notes");
// 				flight.crew				= row.colstr ("crew");
// 				flight.trip				= row.colstr ("trip");
// 				flight.trip_date		= Date.from_iso8601 (row.colstr ("tripdate"));
// 				flight.route.clear ();
// 				flight.route.read (row.colstr ("route"));
//				flight.save ();
//			}
		}

		private class RowIterator {
			
			private HashTable<string, int> columns;
			private CSV.Reader csv;
			private weak string[] current_row;
			
			public RowIterator (FileStream input) {
				int ncol = 256;
				int nbuf = 8192;
				csv = new CSV.Reader ( ncol, nbuf );
				columns = new HashTable<string, int> (str_hash, str_equal);
				csv.instream = input;
				read_column_headers ();
			}
			
			public bool next () {
				if ((current_row = csv.read_row ()) != null) return true;
				return false;
			}
			
			private void read_column_headers () {
				weak string[] fields;
				if ((fields = csv.read_row ()) != null) {
					for (int i = 0; i < fields.length; i++) {
						if (fields[i] != null) {
							// Hash should return zero for not found, offest real index by + 1
							if (columns.lookup (fields[i]) == 0) {
								columns.insert (fields[i].down(), i + 1);
							} else {
								// duplicate key
							}
						}
					}
				}
			}

			public weak string? col (string column_name) {
				int index = columns.lookup (column_name);
				if (index != 0) {
					return current_row[index - 1];
				} else {
					return null;
				}
			}
			
			public weak string colstr (string column_name) {
				weak string s = col (column_name);
				return (s == null) ? "" : s;
			}

		} // end Row
	}
}

public static int main (string[] args) {
	var logbook = new Logbook ();
	var importer = new Importer ();
	if (args[1] == null) return 0;
	if (args[2] == null) return 0;
	logbook.open (args[1]);
	importer.do_import (logbook, FileStream.open(args[2],"r"));
	return 0;
}