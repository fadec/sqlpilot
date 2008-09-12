
#include "logbook.h"
#include "import.h"
#include "lib/csv.h"

#define CSV_BUFSIZE 4096
#define CSV_COLS 128
char csv_buf[CSV_BUFSIZE];
char *csv_row[CSV_COLS];

void import_init(Logbook *logbook)
{
  scripter_init(logbook->import_scripter, GTK_COMBO_BOX(logbook->import_script), GTK_BOX(logbook->import_script_options), "-g");
  scripter_merge_script_dir(logbook->import_scripter, IMPORT_SCRIPT_DIR);
  gtk_combo_box_set_active(GTK_COMBO_BOX(logbook->import_script), 0);
}

static int build_table_from_csv_fh(FILE *in, GtkWidget **ret_view, GtkTreeModel **ret_store, GtkProgressBar *progress)
{
  GType *column_types;
  int n, ncol, nrow;
  int sep = ',';
  GtkListStore *store=NULL;
  GtkWidget *view=NULL;

  if (csv_row_fread(in,
		    (unsigned char *)csv_buf,
		    CSV_BUFSIZE,
		    (unsigned char **)csv_row,
		    CSV_COLS,
		    sep,
		    CSV_QUOTES) > 0) {

    /* Create model with number of columns matching the csv input */
    for(ncol=0; csv_row[ncol]; ncol++);
    column_types = malloc(sizeof(GType) * ncol);
    for (n=0; n<ncol; n++) {
      column_types[n] = G_TYPE_STRING;
    }
    store = gtk_list_store_newv(ncol, column_types);
    free(column_types);

    /* Create view, add colums */
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    view = gtk_tree_view_new_with_model (GTK_TREE_MODEL(store));
    for (n = 0; n < ncol; n++) {
      renderer = gtk_cell_renderer_text_new();
      column = gtk_tree_view_column_new_with_attributes(csv_row[n], renderer, "text", n, NULL);
/*       gtk_tree_view_column_set_sort_column_id(column, n); */
/*       gtk_tree_view_column_set_reorderable(column, TRUE); */
      gtk_tree_view_column_set_resizable(column, TRUE);
      gtk_tree_view_insert_column(GTK_TREE_VIEW(view), column, n);
    }

    /* Populate model */
    GtkTreeIter iter;
    if (progress) {
      gtk_progress_bar_set_fraction(progress, 0.0);
    }

    while (csv_row_fread(in,
			 (unsigned char *)csv_buf,
			 CSV_BUFSIZE,
			 (unsigned char **)csv_row,
			 CSV_COLS,
			 sep,
			 CSV_QUOTES) > 0) {
      gtk_list_store_append(store, &iter);
      for(n = 0; n < ncol; n++) {
	gtk_list_store_set(store, &iter, n, csv_row[n], -1);
      }
      if (!(nrow % 256)) {
	while (gtk_events_pending()) gtk_main_iteration();
      }
      gtk_main_iteration_do(FALSE);
      nrow++;
      if (progress) {};
    }
  }
  g_object_unref(store); /* So only the treeview ref remains - it should be deleted when treeview is destroyed */
  if (ret_store) *ret_store = GTK_TREE_MODEL(store);
  if (ret_view) *ret_view = view;
  return nrow;
}

int import_read_text(Logbook *logbook)
{
  GPid pid;
  int fdin, fdout, fderr;
  FILE *fin=NULL, *fout=NULL, *ferr=NULL;
  GError *error=NULL;
  char *txt;
  GtkWidget *view;
  GtkTreeModel *treemod;
  int nrow;

/*   gchar *script_filename = filename_combo_box_get_current_full_filename(GTK_COMBO_BOX(logbook->import_script)); */
/*   gchar *argv[] = {script_filename, "-a", NULL}; */

  gchar *argv[32];
  argv[0] = scripter_get_script_filename(logbook->import_scripter);
  scripter_get_parameter_values(logbook->import_scripter, argv+1);

  if (g_spawn_async_with_pipes(NULL, argv, NULL, 0, NULL, NULL, &pid, &fdin, &fdout, &fderr, &error)) {
    if (!((fin  = fdopen(fdin, "ab")) &&
	  (fout = fdopen(fdout, "rb")) &&
	  (ferr = fdopen(fderr, "rb")))) {
      fprintf(stderr, "spawn_script fdopen failed\n");
      exit(1);
    }
    
    txt = text_view_get_text(GTK_TEXT_VIEW(logbook->import_input_text));

    assert(fin);
    fputs(txt, fin);
    fclose(fin);

    assert(fout);
    nrow = build_table_from_csv_fh(fout, &view, &treemod, NULL);
    fclose(fout); 
    assert(ferr);
    fclose(ferr);
    g_spawn_close_pid(pid);

    if (logbook->import_interpreted_treeview) gtk_widget_destroy(logbook->import_interpreted_treeview);
    logbook->import_interpreted_treeview = view;
    logbook->import_interpreted_treemodel = treemod;
    gtk_container_add(GTK_CONTAINER(logbook->import_interpreted_sw), view);
    gtk_widget_show(view);
  } else {
    fprintf(stderr, "Can't open import script\n");
  }
/*   g_free(script_filename); */
  return nrow;
}

int import_read_file(Logbook *logbook)
{
  GPid pid;
  int fdin, fdout, fderr;
  FILE *fin=NULL, *fout=NULL, *ferr=NULL;
  GError *error=NULL;
  char *txt;
  GtkWidget *view;
  GtkTreeModel *treemod;
  int nrow;

  gchar *script_filename = filename_combo_box_get_current_full_filename(GTK_COMBO_BOX(logbook->import_script));
  gchar *argv[] = {script_filename, gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(logbook->import_input_file)), NULL};

  if (g_spawn_async_with_pipes(NULL, argv, NULL, 0, NULL, NULL, &pid, &fdin, &fdout, &fderr, &error)) {
    if (!((fin  = fdopen(fdin, "ab")) &&
	  (fout = fdopen(fdout, "rb")) &&
	  (ferr = fdopen(fderr, "rb")))) {
      fprintf(stderr, "spawn_script fdopen failed\n");
      exit(1);
    }
    
    txt = text_view_get_text(GTK_TEXT_VIEW(logbook->import_input_text));

    assert(fin);
    fputs(txt, fin);
    fclose(fin);

    assert(fout);
    nrow = build_table_from_csv_fh(fout, &view, &treemod, NULL);
    fclose(fout); 
    assert(ferr);
    fclose(ferr);
    g_spawn_close_pid(pid);

    if (logbook->import_interpreted_treeview) gtk_widget_destroy(logbook->import_interpreted_treeview);
    logbook->import_interpreted_treeview = view;
    logbook->import_interpreted_treemodel = treemod;
    gtk_container_add(GTK_CONTAINER(logbook->import_interpreted_sw), view);
    gtk_widget_show(view);
  } else {
    fprintf(stderr, "Can't open import script\n");
  }
  g_free(script_filename);
  return nrow;
}

void import_write(Logbook *logbook)
{
  GPid pid;
  int fdin, fdout, fderr;
  FILE *fin=NULL, *fout=NULL, *ferr=NULL;
  GError *error=NULL;

  gchar *argv[] = {"./importcsv",
		   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->import_write_force)) ? "-f" : "",
		   logbook->db_filename,

		   NULL};

  if (g_spawn_async_with_pipes(NULL, argv, NULL, 0, NULL, NULL, &pid, &fdin, &fdout, &fderr, &error)) {
    if (!((fin  = fdopen(fdin, "ab")) &&
	  (fout = fdopen(fdout, "rb")) &&
	  (ferr = fdopen(fderr, "rb")))) {
      fprintf(stderr, "spawn_script fdopen failed\n");
      exit(1);
    }
    store_fwrite_csv(GTK_TREE_VIEW(logbook->import_interpreted_treeview), fin);
    fclose(fin);

    int c;
    GString *gsout, *gserr;
    gsout = g_string_new("");
    gserr = g_string_new("");
    while ((c = fgetc(fout)) != EOF) {
      gsout = g_string_append_c(gsout, c);
    }
    while ((c = fgetc(ferr)) != EOF) {
      gserr = g_string_append_c(gserr, c);
    }    
    fclose(fout);
    fclose(ferr);

    text_view_set_text(GTK_TEXT_VIEW(logbook->import_response_text), gsout->str);
    g_string_free(gsout, FALSE);
    g_string_free(gserr, FALSE);

    logbook->flights_stale = TRUE;
    logbook->aircraft_stale = TRUE;
    logbook->roles_stale = TRUE;
    logbook->types_stale = TRUE;
    logbook->airports_stale = TRUE;
  } else {
    fprintf(stderr, "Could not open database import program\n");
  }
}
