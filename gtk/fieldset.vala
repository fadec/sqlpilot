using Sqlp;

namespace SqlpGtk {
	public abstract class Fieldset <RecordType> : Pane {

		// owns this
		public weak Browser browser { construct set; get; }

		public signal void saved (int64 id);

		public bool edited { get; set; }

		protected RecordType _record;

		public RecordType record {
			get {
				assert (this is Fieldset);
				ensure_record ();
				assert (_record is Record);
				return this._record;
			}
			set {
				// Record has erased type so vala doesn't know to ref it (but it still deletes with unref??)
				_record = value;
				if (value == null) ensure_record ();
				else (_record as Record).ref();
				assert (_record is Record);
				set_fields_from_record ();
				edited = false;
			}
		}

		construct {
			edited = false;
		}

		protected abstract void set_fields_from_record ();
		protected abstract void set_record_from_fields ();

		protected bool save () {
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