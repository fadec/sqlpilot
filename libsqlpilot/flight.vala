namespace SqlPilot {
	public class Flight : Record {

		public Aircraft aircraft;
		public Role role;
		public Airport dep;
		public Airport arr;
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
	}
}