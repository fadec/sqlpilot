using Sqlp;

namespace SqlpGtk {
	public abstract class Fieldset <RecordType> : Pane {

		// owns this
		public weak Browser browser { construct set; get; }

		public signal void saved (int64 id);

		public bool edited { get; set; }

		private RecordType _record;

		public RecordType record {
			get {
				ensure_record ();
				return this._record;
			}
			set {
				this._record = value;
				ensure_record ();
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
			if (r.save ()) {
				saved (r.id);
				edited = false;
				return true;
			}
			return false;
		}

		private void ensure_record () {
			assert (browser != null);
			assert (browser.crud != null);
			if (_record == null) _record = browser.crud.new_record ();
		}
	}
}