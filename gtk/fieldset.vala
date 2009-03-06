using Sqlp;

namespace SqlpGtk {
	public abstract class Fieldset <RecordType> : Pane {

		// owns this
		public weak Browser browser { construct set; get; }

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
			}
		}

		protected abstract void set_fields_from_record ();
		protected abstract void set_record_from_fields ();

		private void ensure_record () {
			assert (browser != null);
			assert (browser.crud != null);
			if (_record == null) _record = browser.crud.new_record ();
		}
	}
}