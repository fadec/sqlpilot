using SqlPilot;

public static int main ( string[] args ) {
	Gtk.init( ref args );		
	if ( args.length > 1 ) {
		try {
			var logbook = new Logbook ( args[1] );
			var window = new MainWindow ();
			window.add_logbook ( logbook );
			window.show_all ();
			Gtk.main ();
		}
		catch (NiceDBError e) {
			stderr.printf ( "%s\n", e.message );
			return 1;
		}
		catch (Error e) {
			stderr.printf ( "%s\n", e.message );
		}
	} else {
		
	}
	return 0;
	}

}