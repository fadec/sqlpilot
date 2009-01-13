namespace SqlPilot {
	public class ModelCrud : Crud {

		public ModelCrud ( Logbook logbook ) {
			base (logbook, "Models");
		}

		public override Record new_record () {
			return new Model ( this ) as Record;
		}
		public Model beget () { return new_record () as Model; }

 		public Model? find_by_id (int64 id) {
			return (record_find_by_id (id) as Model);
 		}


	}
}