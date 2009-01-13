
using Gtk;
using SqlPilot;
namespace SqlPilotGtk {

	public class Editor : Pane {

		private enum State {
			EMPTY,		   // --> treeselect->SELECTED | editing->MODIFIED | editing->INVALID | paste->(MODIFIED | INVALID)
			SELECTED,	   // --> cancel->EMPTY | editing->MODIFIED | editing->INVALID | arm->DELETE_ARMED | copy | paste->(MODIFIED | INVALID)
			MULTIPLE,	   // --> arm->DELETE_ARMED
			MODIFIED,      // --> save->SELECTED | cancel->(SELECTED | EMPTY) | copy | paste->(MODIFIED | INVALID)
			INVALID,	   // --> cancel->SELECTED | cancel->EMPTY | copy | paste->(MODIFIED | INVALID)
			DELETE_ARMED,  // --> unarm->(SELECTED | MULTIPLE) | delete->EMPTY
		}

		protected Pane fieldset;
		protected Pane toolbar;

		private Record _current_record;
		private Record _copied_record;

		private State _state;
		private TreeView _tree_view;
		private TreeSelection _tree_selection;
		private Action _new;
		private Action _save;
		private Action _cancel;
		private Action _arm_delete;
		private Action _delete;
		private Action _copy_record;
		private Action _paste_record;
		private Label  _message;

		public Editor ( string fields_xml, string toolbar_xml ) {
			base ( "data/ui/editor.xml" );
			fieldset = new Pane ( fields_xml );
			add_child ( "fields", fieldset );
			toolbar = new Pane ( toolbar_xml );
			add_child ( "toolbar", toolbar );
		}

		private virtual void load_selection () {
		}
		
		private virtual void save () {
			_current_record.save();
		}
	}
}