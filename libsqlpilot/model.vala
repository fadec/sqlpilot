using Sqlite;
namespace SqlPilot {
	public class Model : Record {

		public string ident;
		public string make;
		public string type;
		public bool airplane;
		public bool rotorcraft;
		public bool glider;
		public bool lta;
		public bool powered_lift;
		public bool ppc;
		public bool weightshift;
		public bool heli;
		public bool gyro;
		public bool airship;
		public bool balloon;
		public bool single;
		public bool multi;
		public bool land;
		public bool sea;
		public bool turbine;
		public bool jet;
		public bool highperf;
		public bool retract;
		public bool complex;
		public bool pressurized;
		public bool large;
		public bool sport;
		public bool ultralight;
		public bool footlaunch;
		public bool sim;
		public bool ftd;
		public bool total;

		public Model (ModelCrud crud) {
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


	}
}