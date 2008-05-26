/************************************************************************/
/* Copyright (C) 2008  Sam Danielson                                    */
/*                                                                      */
/* This file is part of Sqlpilot.				        */
/* 								        */
/* Sqlpilot is free software: you can redistribute it and/or modify     */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation, either version 3 of the License, or    */
/* (at your option) any later version.				        */
/* 								        */
/* Sqlpilot is distributed in the hope that it will be useful,	        */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of       */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        */
/* GNU General Public License for more details.			        */
/* 								        */
/* You should have received a copy of the GNU General Public License    */
/* along with Sqlpilot.  If not, see <http://www.gnu.org/licenses/>.    */
/************************************************************************/

#include "sqlpilot.h"

#ifdef USING_HILDON
#include <hildon-widgets/hildon-program.h>
#include <locale.h>
#include <libintl.h>
#include <libintl.h>
#define _(String) gettext(String)
#define N_(String) (String)
#else
#include <gtk/gtk.h>
#endif
Sqlpilot *sqlpilot;

int
main (int argc, char *argv[])
{
  char *dbfilename;
  int console=FALSE;

#ifdef USING_HILDON
  HildonProgram *program;
  GtkWidget *container;
  GtkWidget *main_menu;
  GtkWidget *item;

  if(!g_thread_supported()) {
    //g_thread_init(NULL);
  }
  setlocale(LC_ALL, "");
  //bind_textdomain_codeset(PACKAGE, "UTF-8"); 
#endif
  
  if (argc < 2) {
    fprintf(stderr, "Usage: sqlpilot <filename>\n");
    exit(0);
  }

  if (*argv[1] == '-') {
    if (argc < 3) {
	fprintf(stderr, "duh\n");
	exit(1);
    }
    dbfilename = argv[2];
    console = TRUE;
  } else {
    dbfilename = argv[1];
  }

  if (console) {
    
  } else {
    gtk_init (&argc, &argv);
    if ((sqlpilot = sqlpilot_new(dbfilename)) == NULL) exit(1);

#ifdef USING_HILDON
    /*  */
    program = HILDON_PROGRAM(hildon_program_get_instance());
    container = sqlpilot->window;
    sqlpilot->window = hildon_window_new();
    //g_signal_connect_swapped(sqlpilot->window ,"destroy", GTK_SIGNAL_FUNC(quit_program),ui_main);
    gtk_widget_reparent(container, sqlpilot->window);

    hildon_program_add_window(program, HILDON_WINDOW(sqlpilot->window));
    g_set_application_name(_("SqlPilot"));
    /* Hildon Menu */
    main_menu = gtk_menu_new();

    item = gtk_menu_item_new_with_label(_("New"));
    //g_signal_connect_swapped( item,"activate",GTK_SIGNAL_FUNC(new_1_player_game),ui_main);
    gtk_menu_append(main_menu, item);

    item = gtk_menu_item_new_with_label(_("Yay"));
    gtk_menu_append(main_menu, item);

    item = gtk_menu_item_new_with_label(_("Hello"));
    gtk_menu_append(main_menu, item);

    item = gtk_menu_item_new_with_label(_("Quit"));
    gtk_menu_append(main_menu, item);

    hildon_window_set_menu(HILDON_WINDOW(sqlpilot->window), GTK_MENU(main_menu));

    gtk_widget_show_all(GTK_WIDGET(main_menu));
#endif
    gtk_widget_show (sqlpilot->window);

    gtk_main ();
    sqlpilot_finalize (sqlpilot);    
  }

  return 0;
}
