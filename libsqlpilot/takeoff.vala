using Sqlite;
namespace Sqlp {
	public class Takeoff : Record {

		public int64 flight_id;
		private Flight? _flight;
		public Flight? flight {
			get {
				if (_flight == null && flight_id != 0) {
					_flight = (table.database as Logbook).flight.find_by_id (flight_id) as Flight?;
				}
				return _flight;
			}
			set {
				_flight = value;
				flight_id = (value == null) ? 0 : value.id;
			}
		}

		public int64 airport_id;
		private Airport? _airport;
		public Airport? airport {
			get {
				if (_airport == null && airport_id != 0) {
					_airport = (table.database as Logbook).airport.find_by_id (airport_id) as Airport?;
				}
				return _airport;
			}
			set {
				_airport = value;
				airport_id = (value == null) ? 0 : value.id;
			}
		}

		public int64 surface_id;
		private Surface? _surface;
		public Surface? surface {
			get {
				if (_surface == null && surface_id != 0) {
					_surface = (table.database as Logbook).surfaces.find_by_id (surface_id) as Surface?;
				}
				return _surface;
			}
			set {
				_surface = value;
				surface_id = (value == null) ? 0 : value.id;
			}
		}

		// A default here makes the vala compiler segfault
		// public Ordinal sequence { get; set; default = Ordinal.invalid (); }
		public Ordinal sequence { get; set; }

		public bool night { get; set; }
		public string runway { get; set; }
		public double crosswind { get; set; }
		public double visibility { get; set; }
		public bool aborted { get; set; }

		construct {
			this.sequence = Ordinal.invalid ();
		}

		protected override int bind_for_save (Statement stmt) {
			var i = 1;
			stmt.bind_nonzero_int64 (i++, flight_id);
			stmt.bind_nonzero_int64 (i++, airport_id);
			stmt.bind_nonzero_int64 (i++, surface_id);
			sequence.bind_to_stmt   (stmt, i++);
			stmt.bind_int (i++, (int) night);
			stmt.bind_nonempty_text (i++, runway);
			stmt.bind_double (i++, crosswind);
			stmt.bind_double (i++, visibility);
			stmt.bind_int (i++, (int) aborted);
			return i;
		}

		protected override void set_from_stmt (Statement stmt) {
			var i = 1;
			flight_id = stmt.column_int64 (i++);
			airport_id = stmt.column_int64 (i++);
			surface_id = stmt.column_int (i++);
			sequence = Ordinal.from_stmt (stmt, i++);
		    night = (bool) stmt.column_int (i++);
			runway = empty_if_null (stmt.column_text (i++));
			crosswind = stmt.column_double (i++);
			visibility = stmt.column_double (i++);
			aborted = (bool) stmt.column_int (i++);
		}

		protected override void before_save () {
			adjust_sequence ();
		}

		private void adjust_sequence () {
			if (is_new () && ! sequence.valid ()) {
				sequence.set ((table as TakeoffTable).next_sequence (this.flight_id));
				return;
			}

			var takeoff_table = table as TakeoffTable;

			if (sequence.valid ()) {
				var old_self = takeoff_table.find_by_id (id) as Takeoff;
				var sequence_conflict = takeoff_table.find_by_sequence (sequence.get ());

				if (sequence_conflict != null && sequence_conflict.id != this.id) {
					// have conflict
					if (old_self != null && old_self.sequence.get () != sequence_conflict.sequence.get ()) {
						// swap sequences
						sequence_conflict.sequence = old_self.sequence;
						// nuke old_self sequence for unique constraint with sequence_conflict
						old_self.sequence = Ordinal.invalid ();
						old_self.save ();
					} else {
						// cannot swap sequences
						sequence_conflict.sequence = Ordinal.invalid ();
					}
					sequence_conflict.save ();
				}
			} else {
				sequence.set (takeoff_table.next_sequence (this.flight_id));
			}
		}

	}
}
