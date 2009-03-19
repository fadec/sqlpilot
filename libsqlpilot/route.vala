// Collection class for flight routing

using GLib;
namespace Sqlp {
	public class Route : Object {

		private Flight? _flight;
		public Flight flight {
			get { return _flight; }
			set {
				_flight = value;
				foreach (Routing r in routings) {
					r.flight = _flight;
				}
			}
		}

		public uint length {
			get { return routings.length (); }
		}

		private List<Routing> routings;
		private RoutingTable table;

		public Route (RoutingTable rtable) {
			table = rtable;
			clear ();
		}

		public string to_string_icao () {
			return "bla icao";
		}
		public string to_string_iata () {
			return "bla iata";
		}
		
		public void clear () {
			routings = new List<Routing> ();
		}

		public Route lookup () {
			if (_flight != null && (! _flight.is_new ())) routings = table.find_by_flight (_flight);
			return this;
		}

		public void append_airport (Airport airport) {
			var routing = table.new_record ();
			routing.airport = airport;
			routings.append (routing);
		}

		public void append_maybe_airport (Airport? airport) {
			if (airport == null || ! (airport is Airport)) return;
			append_airport (airport);
		}

		public Route read (string? str) {
			clear ();
			if (str != null) {
				foreach (string ident in str.split (" ")) {
					append_airport (table.database.airport.find_by_ident (ident));
				}
			}
			return this;
		}

		public Route write () {
			return this;
		}

		public bool save () {
			int seq = 1;
			foreach (Routing r in routings) {
				r.flight = _flight;
				r.seq = seq++;
				r.save ();
			}
			return true;
		}
	}
}
