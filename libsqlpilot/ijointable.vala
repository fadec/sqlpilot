
namespace Sqlp {
	public abstract interface IJoinTable : Table  {
		public abstract string parent_id_column_name { get; }
		public abstract string child_id_column_name { get; }
	}
}
