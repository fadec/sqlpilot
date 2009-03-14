using Sqlp;

namespace SqlpGtk {
	public abstract class Fieldset <RecordType> : Pane {

		// owns this
		public weak Browser browser { construct set; get; }

		public signal void saved (int64 id);

		public bool edited { get; set; }

		private bool save_disabled;

		protected Record _record;
		public RecordType record {
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
				save_disabled = true;
				set_fields_from_record ();
				save_disabled = false;
				edited = false;
			}
		}

		construct {
			edited = false;
		}

		protected abstract void set_fields_from_record ();
		protected abstract void set_record_from_fields ();

		public bool save () {
			if (save_disabled) return false;
		    weak Record r = record as Record;
			assert (r is Record);
			if (r.save ()) {
				saved (r.id);
				edited = false;
				return true;
			}
			return false;
		}

		private void ensure_record () {
 			assert (browser != null);
 			assert (browser.crud is Crud);
			if (_record == null) _record = browser.crud.new_record ();
 			assert (_record is Record);
		}
	}
}