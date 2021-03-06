
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <glib.h>
#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include <gtksourceview/gtksourceview.h>
#include <gtksourceview/gtksourcebuffer.h>
#include <gtksourceview/gtksourcelanguage.h>

#include "interface.h"
#include "callbacks.h"
#include "griffon_defs.h"
#include "griffon_gtk_utils.h"
#include "griffon_proj.h"
#include "rox_strings.h"
#include "griffon_config.h"

static GtkWidget *ent_project_name;
static GtkWidget *ent_dir_makefile;
static GtkWidget *ent_dir_source;
static GtkWidget *ent_file_executable;

static GtkWidget* mk_page_main (t_tea_project *p)
{
  GtkWidget *page = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);

  gtk_widget_show (page);
  ent_project_name = tea_text_entry (GTK_WIDGET(page), _("Project name"), p->project_name);
  ent_dir_makefile = tea_dir_selector (page, _("Makefile directory"), p->dir_makefile);
  ent_dir_source = tea_dir_selector (page, _("Source directory"), p->dir_source);
  ent_file_executable = tea_text_entry (page, _("Target executable"), p->file_executable);
  
gtk_text_buffer_insert_at_cursor(GTK_TEXT_BUFFER(buffer_projet), "\n\n", -1);
gtk_text_buffer_insert_at_cursor(GTK_TEXT_BUFFER(buffer_projet), "\nfile_executable=", -1);
gtk_text_buffer_insert_at_cursor(GTK_TEXT_BUFFER(buffer_projet), p->file_executable, -1);
gtk_text_buffer_insert_at_cursor(GTK_TEXT_BUFFER(buffer_projet), "\nproject_name=", -1);
gtk_text_buffer_insert_at_cursor(GTK_TEXT_BUFFER(buffer_projet), p->project_name, -1);
gtk_text_buffer_insert_at_cursor(GTK_TEXT_BUFFER(buffer_projet), "\ndir_makefile=", -1);
gtk_text_buffer_insert_at_cursor(GTK_TEXT_BUFFER(buffer_projet), p->dir_makefile, -1);
gtk_text_buffer_insert_at_cursor(GTK_TEXT_BUFFER(buffer_projet), "\ndir_source=", -1);
gtk_text_buffer_insert_at_cursor(GTK_TEXT_BUFFER(buffer_projet), p->dir_source, -1);

  return page;
}


t_tea_project* tea_proj_new (void)
{
  t_tea_project *p = g_malloc (sizeof (t_tea_project)); 

  p->project_name = g_strdup ("noname");
  p->project_file_name = NULL;
  p->dir_makefile = NULL;
  p->dir_source = NULL;
  p->file_executable = NULL;

  return p;
} 


t_tea_project* tea_proj_clear_new (t_tea_project *p)
{
  tea_proj_free (p);
  return tea_proj_new ();
} 


t_tea_project* tea_proj_open (gchar *name)
{
  t_tea_project *r = tea_proj_new ();
  GHashTable *ht = load_file_to_hashtable (name);

  r->project_name = ht_get_gchar_value (ht, "project_name", "noname");
  r->project_file_name = ch_str (r->project_file_name, name);
  r->dir_makefile = ht_get_gchar_value (ht, "dir_makefile", "noname");
  r->dir_source = ht_get_gchar_value (ht, "dir_source", "noname");
  r->file_executable = ht_get_gchar_value (ht, "file_executable", "noname");

	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(filechooserwidget2) ,r->dir_source);

  g_hash_table_destroy (ht); 
  return r;
}


void tea_proj_save (t_tea_project *p)
{
  if (! p)
     return;

  if (! p->project_file_name)
     return; 

  GList *l = NULL;
  
  l = add_to_glist_combined_str (l, "file_executable", p->file_executable);
  l = add_to_glist_combined_str (l, "project_name", p->project_name);
  l = add_to_glist_combined_str (l, "project_file_name", p->project_file_name);
  l = add_to_glist_combined_str (l, "dir_makefile", p->dir_makefile);
  l = add_to_glist_combined_str (l, "dir_source", p->dir_source);

  glist_save_to_file (l, p->project_file_name);
  g_list_free (l); 
}


void tea_proj_save_as (t_tea_project *p, gchar *name)
{
  if (! p)
     return;

  if (! name)
     return;

  p->project_file_name = ch_str (p->project_file_name, name);

  GList *l = NULL;

gtk_text_buffer_insert_at_cursor(GTK_TEXT_BUFFER(buffer_projet), "\nProeject file name :\n", -1);
gtk_text_buffer_insert_at_cursor(GTK_TEXT_BUFFER(buffer_projet), p->project_file_name, -1);
gtk_text_buffer_insert_at_cursor(GTK_TEXT_BUFFER(buffer_projet), "\n\n", -1);
gtk_text_buffer_insert_at_cursor(GTK_TEXT_BUFFER(buffer_projet), "\nfile_executable=", -1);
gtk_text_buffer_insert_at_cursor(GTK_TEXT_BUFFER(buffer_projet), p->file_executable, -1);
gtk_text_buffer_insert_at_cursor(GTK_TEXT_BUFFER(buffer_projet), "\nproject_name=", -1);
gtk_text_buffer_insert_at_cursor(GTK_TEXT_BUFFER(buffer_projet), p->project_name, -1);
gtk_text_buffer_insert_at_cursor(GTK_TEXT_BUFFER(buffer_projet), "\ndir_makefile=", -1);
gtk_text_buffer_insert_at_cursor(GTK_TEXT_BUFFER(buffer_projet), p->dir_makefile, -1);
gtk_text_buffer_insert_at_cursor(GTK_TEXT_BUFFER(buffer_projet), "\ndir_source=", -1);
gtk_text_buffer_insert_at_cursor(GTK_TEXT_BUFFER(buffer_projet), p->dir_source, -1);
  
  l = add_to_glist_combined_str (l, "file_executable", p->file_executable);
  l = add_to_glist_combined_str (l, "project_name", p->project_name);
  l = add_to_glist_combined_str (l, "project_file_name", p->project_file_name);
  l = add_to_glist_combined_str (l, "dir_makefile", p->dir_makefile);
  l = add_to_glist_combined_str (l, "dir_source", p->dir_source);

  glist_save_to_file (l, p->project_file_name);
  g_list_free (l); 
}


void tea_proj_free (t_tea_project *p)
{
  if (! p)
     return;

  g_free (p->project_name);
  g_free (p->project_file_name);
  g_free (p->dir_makefile);
  g_free (p->dir_source);
  g_free (p->file_executable);
  g_free (p);
}

//xmms now plays Depeche Mode - Enjoy the Silence
void tea_proj_run (t_tea_project *p)
{
  if (! p)
     return;

  if (! p->file_executable)
     return; 

  gchar *f = g_strconcat (p->dir_source, p->file_executable, NULL);
  g_spawn_command_line_async (f, NULL);
  g_free (f);
}


GtkWidget* create_proj_props_window (t_tea_project *p)
{
  if (! p)
    return NULL; 

  wnd_proj_props = gtk_window_new (GTK_WINDOW_TOPLEVEL);

  gtk_window_set_title (GTK_WINDOW (wnd_proj_props), _("Project properties"));

  GtkWidget *vbox1 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_show (vbox1);
  gtk_container_add (GTK_CONTAINER (wnd_proj_props), vbox1);

  GtkWidget *notebook = gtk_notebook_new ();         
  gtk_notebook_set_tab_pos (GTK_NOTEBOOK(notebook), GTK_POS_LEFT);

  gtk_widget_show (notebook);

  gtk_container_add (GTK_CONTAINER (vbox1), notebook);

  gtk_notebook_append_page (GTK_NOTEBOOK(notebook), mk_page_main (p), gtk_label_new (_("Main")));
  
  GtkWidget *hbox1 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_show (hbox1);
  gtk_container_add (GTK_CONTAINER (vbox1), hbox1);

  GtkWidget *bt_close = gtk_button_new_from_stock (GTK_STOCK_CLOSE);
  gtk_widget_show (bt_close);
  gtk_container_add (GTK_CONTAINER (hbox1), bt_close);
  g_signal_connect (bt_close, "clicked", G_CALLBACK (cb_on_bt_close), wnd_proj_props);

  t_ppair *pair = g_malloc (sizeof (t_ppair)); 
  pair->a = wnd_proj_props;
  pair->b = p;

  GtkWidget *bt_apply = gtk_button_new_from_stock (GTK_STOCK_OK);
  gtk_widget_show (bt_apply);
  gtk_container_add (GTK_CONTAINER (hbox1), bt_apply);
 // g_signal_connect (bt_apply, "clicked", G_CALLBACK (cb_proj_on_bt_apply), pair);

  gtk_window_set_position (GTK_WINDOW(wnd_proj_props), GTK_WIN_POS_CENTER);   
  gtk_window_set_modal (GTK_WINDOW(wnd_proj_props), TRUE);

  gtk_widget_show (wnd_proj_props);
  return wnd_proj_props;
}
