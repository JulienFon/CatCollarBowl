
#include <stdlib.h>
#include <jansson.h>
#include <gtk/gtk.h>
#include <glib.h>
#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>

#include <stdio.h>  // Standard input/output definitions
#include <unistd.h> // UNIX standard function definitions
#include <fcntl.h>  // File control definitions
#include <errno.h>  // Error number definitions
#include <string.h> // String function definitions

GObject *p_builder;

static void change_page(GtkWidget *widget, gpointer page)
{
    GObject *p_notebook = gtk_builder_get_object(p_builder, "Notebook");
    gtk_notebook_set_current_page(p_notebook, gtk_notebook_page_num(p_notebook, page));
}

void set_navigation()
{
    GObject *p_goto_alarm_button = gtk_builder_get_object(p_builder, "go_to_alarm_button");
    GObject *p_alarm_page = gtk_builder_get_object(p_builder, "clock_page");
    g_signal_connect(p_goto_alarm_button, "clicked", G_CALLBACK(change_page), p_alarm_page);

    GObject *p_goto_clock_button = gtk_builder_get_object(p_builder, "go_to_clock_button");
    GObject *p_clock_page = gtk_builder_get_object(p_builder, "clock_page");
    g_signal_connect(p_goto_clock_button, "clicked", G_CALLBACK(change_page), p_clock_page);
}

void set_alarm(char *selector, int increment)
{
}

int main(int argc, char **argv)
{
    GError *p_err = NULL;
    /* Initialisation de GTK+ */
    gdk_threads_init();
    gdk_threads_enter();
    gtk_init(&argc, &argv);

    /* Creation d'un nouveau GtkBuilder */
    p_builder = gtk_builder_new();

    if (p_builder != NULL)
    {
        /* Chargement du XML dans p_builder */
        gtk_builder_add_from_file(p_builder, "Heure.glade", &p_err);

        if (p_err == NULL)
        {
            /* Recuperation d'un pointeur sur la fenetre. */
            GObject *p_win = gtk_builder_get_object(p_builder, "MainWindow");
            set_navigation();
        }
    }
}