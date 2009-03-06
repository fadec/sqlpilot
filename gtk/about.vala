using GLib;
using Gtk;

namespace SqlpGtk {
	public class SqlpilotAboutDialog : AboutDialog {
		public SqlpilotAboutDialog (Window transient_for) {
			this.transient_for = transient_for;
			this.modal = false;
			this.destroy_with_parent = true;
			this.program_name = "Sqlpilot";
			this.version = "0.0.0";
			this.copyright = "Copyright © 2008-2009 Samuel Danielson";
			this.authors = {"Samuel Danielson"};
			this.comments = "Logbook Software for the Professional Pilot";
			this.wrap_license = true;
			this.license = """Sqlpilot is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Sqlpilot is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Sqlpilot.  If not, see <http://www.gnu.org/licenses/>.""";
		}

		construct {
			this.delete_event += (ad) => { ad.hide_on_delete (); };
			this.response += (ad) => { ad.hide (); };
		}

	}
}