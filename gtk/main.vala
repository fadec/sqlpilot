using SqlpGtk;

public static int main ( string[] args ) {
	Gtk.init( ref args );		
	if ( args.length > 1 ) {
		try {
			var book = new Book ( args[1] );
			var application = new Application ();
			application.add_book ( book );
			application.top_widget.show ();
			Gtk.main ();
		}
		catch (Error e) {
			stderr.printf ( "%s\n", e.message );
		}
	} else {
		
	}
	return 0;
	}

}