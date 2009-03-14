using Sqlite;
namespace Sqlp {
	public class Model : Record <ModelCrud> {

		public string abbreviation;
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
		public bool high_perf;
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

		public override int bind_for_save (Statement stmt) {
			var i = 1;
			stmt.bind_nonempty_text (i++, abbreviation);
			stmt.bind_nonempty_text (i++, make);
			stmt.bind_nonempty_text (i++, type);
			stmt.bind_int (i++, (int) airplane);
			stmt.bind_int (i++, (int) rotorcraft);
			stmt.bind_int (i++, (int) glider);
			stmt.bind_int (i++, (int) lta);
			stmt.bind_int (i++, (int) powered_lift);
			stmt.bind_int (i++, (int) ppc);
			stmt.bind_int (i++, (int) weightshift);
			stmt.bind_int (i++, (int) heli);
			stmt.bind_int (i++, (int) gyro);
			stmt.bind_int (i++, (int) airship);
			stmt.bind_int (i++, (int) balloon);
			stmt.bind_int (i++, (int) single);
			stmt.bind_int (i++, (int) multi);
			stmt.bind_int (i++, (int) land);
			stmt.bind_int (i++, (int) sea);
			stmt.bind_int (i++, (int) turbine);
			stmt.bind_int (i++, (int) jet);
			stmt.bind_int (i++, (int) high_perf);
			stmt.bind_int (i++, (int) retract);
			stmt.bind_int (i++, (int) complex);
			stmt.bind_int (i++, (int) pressurized);
			stmt.bind_int (i++, (int) large);
			stmt.bind_int (i++, (int) sport);
			stmt.bind_int (i++, (int) ultralight);
			stmt.bind_int (i++, (int) footlaunch);
			stmt.bind_int (i++, (int) sim);
			stmt.bind_int (i++, (int) ftd);
			stmt.bind_int (i++, (int) total);
			return i;
		}

		public override void set_from_stmt (Statement stmt) {
			var i = 1;
			abbreviation		= stmt.column_text (i++);
			make				= stmt.column_text (i++);
			type				= stmt.column_text (i++);
			airplane			= (bool) stmt.column_int (i++);
			rotorcraft			= (bool) stmt.column_int (i++);
			glider				= (bool) stmt.column_int (i++);
			lta					= (bool) stmt.column_int (i++);
			powered_lift		= (bool) stmt.column_int (i++);
			ppc					= (bool) stmt.column_int (i++);
			weightshift			= (bool) stmt.column_int (i++);
			heli				= (bool) stmt.column_int (i++);
			gyro				= (bool) stmt.column_int (i++);
			airship				= (bool) stmt.column_int (i++);
			balloon				= (bool) stmt.column_int (i++);
			single				= (bool) stmt.column_int (i++);
			multi				= (bool) stmt.column_int (i++);
			land				= (bool) stmt.column_int (i++);
			sea					= (bool) stmt.column_int (i++);
			turbine				= (bool) stmt.column_int (i++);
			jet					= (bool) stmt.column_int (i++);
			high_perf			= (bool) stmt.column_int (i++);
			retract				= (bool) stmt.column_int (i++);
			complex				= (bool) stmt.column_int (i++);
			pressurized			= (bool) stmt.column_int (i++);
			large				= (bool) stmt.column_int (i++);
			sport				= (bool) stmt.column_int (i++);
			ultralight			= (bool) stmt.column_int (i++);
			footlaunch			= (bool) stmt.column_int (i++);
			sim					= (bool) stmt.column_int (i++);
			ftd					= (bool) stmt.column_int (i++);
			total				= (bool) stmt.column_int (i++);
		}

	}
}