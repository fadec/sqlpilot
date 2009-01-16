// Collection class for flight routing

using GLib;
namespace SqlPilot {
	public class Route {

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

		private List<Routing> routings;
		private RoutingCrud crud;

		public Route (RoutingCrud rcrud) {
			crud = rcrud;
			clear ();
		}

		public void clear () {
			routings = new List<Routing> ();
		}

		public Route lookup () {
			if (_flight != null) routings = crud.find_by_flight (_flight);
			return this;
		}

		public void append_airport (Airport airport) {
			var routing = new Routing (crud);
			routing.airport = airport;
			routings.append (routing);
		}

		public Route read (string str) {
			clear ();

			foreach (string ident in str.split ("-")) {
				append_airport (crud.logbook.airport.find_by_ident (ident));
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
