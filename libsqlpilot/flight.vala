using Sqlite;
namespace SqlPilot {
	public class Flight : Record {

		public int64 aircraft_id;
		private Aircraft? _aircraft;
		public Aircraft? aircraft {
			get {
				if (_aircraft == null && aircraft_id != 0) {
					_aircraft = crud.logbook.aircraft.find_by_id (aircraft_id);
				}
				return _aircraft;
			}
			set { _aircraft = value;
				aircraft_id = value.id;
			}
		}

		public int64 role_id;
		public Role role;

		public int64 dep_id;
		public Airport dep;

		public int64 arr_id;
		public Airport arr;

		private List<Routing> _routing;
		public List<Routing> routing {
			get {
				_routing = crud.logbook.routing.find_by_flight (this);
				return _routing;
			}
		}

		public Date date;
		public int leg;
		public TimeOfDay aout;
		public TimeOfDay ain;
		public Duration dur;
		public TimeOfDay night;
		public TimeOfDay inst;
		public TimeOfDay sim_inst;
		public bool hold;
		public string aprch;
		public bool xc;
		public int dland;
		public int nland;
		public string crew;
		public string notes;
		public string fltno;
		public TimeOfDay sout;
		public TimeOfDay sin;
		public Duration sdur;
		public string trip;
		public Date trip_date;

		public Flight ( FlightCrud crud ) {
			base (crud);
		}

		public override int bind_for_save (Statement stmt) {
			var i = 1;
			// bind here and return id column
			return i;
		}

		public override void set_from_stmt (Statement stmt) {
			var i = 0;
		}

		protected override bool save_dependencies () {
			if (aircraft != null) aircraft.save ();
			if (role != null) role.save ();
			if (dep != null) dep.save ();
			if (arr != null) arr.save ();
			return true;
		}

		protected override bool save_dependents () {
			foreach (Routing r in routing) {
				r.save ();
			}
			return true;
		}
	}
}