using Sqlp;

namespace SqlpGtk {
	public abstract class Fieldset : Pane {

		// owns this
//		public unowned Browser browser { construct set; get; }
		public unowned Sqlp.Table table { construct; get; }

		public bool edited { get; set; }

		public bool setting_record { get; private set; }

		protected Record _record;
		public Record record {
			get {
				assert (this is Fieldset);
				ensure_record ();
				assert (_record is Record);
				return this._record;
			}
			set {
				_record = value as Record;
				if (value == null) ensure_record ();
				assert (_record is Record);
				setting_record = true;
				set_fields_from_record ();
				edited = false;
				setting_record = false;
			}
		}

		construct {
			edited = false;
		}

		protected abstract void set_fields_from_record ();

		public bool save () {
			if (setting_record) return true;
		    weak Record r = record as Record;
			assert (r is Record);
			if (r.save ()) {
				edited = false;
				return true;
			}
			return false;
		}

		private void ensure_record () {
 			assert (table is Table);
			if (_record == null) _record = table.new_record ();
 			assert (_record is Record);
		}
	}
}