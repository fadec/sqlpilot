using GLib;
using CSV;
using SqlPilot; // for main

namespace SqlPilot {
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
			Airport?  dep;
			Airport?  arr;
			while (row.next ()) {
			
				var date        = Date.from_iso8601 (row.col ("date"));
				var fltno       = row.col ("fltno");
				var dep_ident   = row.col ("dep");
				var arr_ident   = row.col ("arr");
				var fleetno     = row.col ("fleetno");
				var model_ident = row.col ("model");
				var role_ident  = row.col ("role");

				flight = logbook.flight.find_by_date_fltno_dep_arr (date.to_iso8601 (),
																	fltno,
																	dep_ident,
																	arr_ident);
				if (flight == null) {
					stderr.printf("new\n");
					flight       = logbook.flight.beget ();
					flight.date  = date;
					flight.fltno = fltno;
					flight.dep   = logbook.airport.find_by_ident (dep_ident);
					flight.arr   = logbook.airport.find_by_ident (arr_ident);
				} else stderr.printf("update flight_id = %d\n", (int)flight.id);

				if ((aircraft =
					 logbook.aircraft.find_by_fleetno (fleetno)) == null) {
					aircraft = logbook.aircraft.beget ();
					aircraft.fleetno = fleetno;
				}

				if ((model =
					 logbook.model.find_by_ident (model_ident)) == null) {
					model = logbook.model.beget ();
					model.ident = model_ident;
				}
				
				if ((role =
					 logbook.role.find_by_ident (role_ident)) == null) {
					role = logbook.role.beget ();
					role.ident = role_ident;
				}

				if ((dep =
					 logbook.airport.find_by_ident (dep_ident)) == null) {
					dep = logbook.airport.beget ();
					dep.set_ident (dep_ident);
				}

				if ((arr =
					 logbook.airport.find_by_ident (arr_ident)) == null) {
					arr = logbook.airport.beget ();
					arr.set_ident (arr_ident);
				}

				flight.role				= role;
				flight.aircraft			= aircraft;
				aircraft.model			= model;
				flight.dep				= dep;
				flight.arr				= arr;
				flight.leg				= row.col ("leg").to_int ();
				flight.aout				= TimeOfDay.from_timezone_time (dep.timezone, row.col ("aout"));
				flight.ain				= TimeOfDay.from_timezone_time (arr.timezone, row.col ("ain"));
				flight.sout				= TimeOfDay.from_timezone_time (dep.timezone, row.col ("sout"));
				flight.sin				= TimeOfDay.from_timezone_time (arr.timezone, row.col ("sin"));
				flight.dur				= Duration.from_string (row.col ("dur"));
				flight.sdur				= Duration.from_string (row.col ("sdur"));
				flight.night			= Duration.from_string (row.col ("night"));
				flight.inst				= Duration.from_string (row.col ("inst"));
				flight.sim_inst			= Duration.from_string (row.col ("siminst"));
				flight.aprch			= row.col ("aprch");
				flight.xc				= str_to_bool (row.col ("xc"));
				flight.hold				= str_to_bool (row.col ("hold"));
				flight.dland			= row.col ("dland").to_int ();
				flight.nland			= row.col ("nland").to_int ();
				flight.notes			= row.col ("notes");
				flight.crew				= row.col ("crew");
				flight.trip				= row.col ("trip");
				flight.trip_date		= Date.from_iso8601 (row.col ("tripdate"));
				flight.route.clear ();
				flight.route.read (row.col ("route"));
				flight.save ();
			}
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