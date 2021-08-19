
#include <stdlib.h>
#include <jansson.h>
#include <gtk/gtk.h>
#include <glib.h>
#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>
#include <time.h>

#include <stdio.h>  // Standard input/output definitions
#include <unistd.h> // UNIX standard function definitions
#include <fcntl.h>  // File control definitions
#include <errno.h>  // Error number definitions
#include <string.h> // String function definitions

GObject *p_builder = NULL;
time_t current_time;
time_t alarm_time;
int alarm_m;
int alarm_h;

struct time_set
{
    char selector;
    int increment
};

char *itoa(int num, char *str)
{
    if (str == NULL)
    {
        return NULL;
    }
    sprintf(str, "%02d", num);
    return str;
}

static void change_page(GtkWidget *widget, gpointer page)
{
    GObject *p_notebook = gtk_builder_get_object(p_builder, "Notebook");
    gtk_notebook_set_current_page(p_notebook, gtk_notebook_page_num(p_notebook, page));
}

void set_navigation()
{

    // alarm setup navigation button
    GObject *p_goto_alarm_button = gtk_builder_get_object(p_builder, "go_to_alarm_button");
    GObject *p_alarm_page = gtk_builder_get_object(p_builder, "alarm_page");
    g_signal_connect(p_goto_alarm_button, "clicked", G_CALLBACK(change_page), p_alarm_page);

    // clock navigation button
    GObject *p_goto_clock_button = gtk_builder_get_object(p_builder, "go_to_clock_button");
    GObject *p_clock_page = gtk_builder_get_object(p_builder, "clock_page");
    g_signal_connect(p_goto_clock_button, "clicked", G_CALLBACK(change_page), p_clock_page);
    change_page(gtk_builder_get_object(p_builder, "Notebook"), p_clock_page);
}

// void set_alarm(char selector[1], int increment)
void change_alarm_m(GtkWidget *widget, gpointer data)
{
    alarm_m = alarm_m + data;
    if (alarm_m == 60)
    {
        alarm_m = 0;
        alarm_h++;
        if (alarm_h == 24)
            alarm_h = 0;
    }
    if (alarm_m == -1)
    {
        alarm_m = 59;
        alarm_h--;
    }
    set_alarm();
}
void change_alarm_h(GtkWidget *widget, gpointer data)
{
    alarm_h = alarm_h + data;
    if (alarm_h == 24)
        alarm_h = 0;
    if (alarm_h == -1)
        alarm_h = 23;

    set_alarm();
}

void set_alarm()
{
    GObject *p_alarm_h_label = gtk_builder_get_object(p_builder, "alarm_hour_label");
    GObject *p_alarm_m_label = gtk_builder_get_object(p_builder, "alarm_minute_label");
    GObject *p_alarm_label = gtk_builder_get_object(p_builder, "alarm_value");
    char hours[3];
    if (itoa(alarm_h, hours) != NULL)
        gtk_label_set_text(p_alarm_h_label, hours);
    char minutes[3];
    if (itoa(alarm_m, minutes) != NULL)
        gtk_label_set_text(p_alarm_m_label, minutes);
    char concat[15];
    sprintf(concat, "Alarme : %s:%s", hours, minutes);
    gtk_label_set_text(p_alarm_label, concat);
}
void turn_motor()
{
    printf("motor turning\n");
}

void call_animal()
{
    printf("sending sms\n");
}
void call_animal_callback(GtkWidget *widget, gpointer data)
{
    call_animal();
}

void set_alarm_buttons()
{
    // init alarm values
    time(&alarm_time);
    struct tm *tm_alarm = gmtime(&alarm_time);
    localtime(&alarm_time);
    alarm_h = tm_alarm->tm_hour;
    alarm_m = tm_alarm->tm_min;
    set_alarm();

    // init alarm button callbacks
    GObject *p_incr_h_btn = gtk_builder_get_object(p_builder, "alarm_hour_increment");
    GObject *p_decr_h_btn = gtk_builder_get_object(p_builder, "alarm_hour_decrement");
    GObject *p_incr_m_btn = gtk_builder_get_object(p_builder, "alarm_minute_increment");
    GObject *p_decr_m_btn = gtk_builder_get_object(p_builder, "alarm_minute_decrement");

    g_signal_connect(p_incr_m_btn, "clicked", G_CALLBACK(change_alarm_m), 1);
    g_signal_connect(p_decr_m_btn, "clicked", G_CALLBACK(change_alarm_m), -1);
    g_signal_connect(p_incr_h_btn, "clicked", G_CALLBACK(change_alarm_h), 1);
    g_signal_connect(p_decr_h_btn, "clicked", G_CALLBACK(change_alarm_h), -1);

    GObject *p_call_animal_btn = gtk_builder_get_object(p_builder, "call_cat");
    g_signal_connect(p_call_animal_btn, "clicked", G_CALLBACK(call_animal_callback), NULL);
}

static void cb_quit(GtkWidget *p_wid, gpointer p_data)
{
    gtk_main_quit();
}

static void *getTime(void *arg)
{
    GObject *p_time_label = gtk_builder_get_object(p_builder, "current_time");
    while (1)
    {
        time(&current_time);
        struct tm *tm_time = gmtime(&current_time);
        localtime(&current_time);
        char time_display[10];
        snprintf(time_display, sizeof(time_display), "%02d:%02d:%02d", tm_time->tm_hour, tm_time->tm_min, tm_time->tm_sec);
        if (tm_time->tm_hour == alarm_h && tm_time->tm_min == alarm_m && tm_time->tm_sec == 0)
        {
            turn_motor();
            call_animal();
        }
        gtk_label_set_text(p_time_label, time_display);
        sleep(1);
    }
    return NULL;
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
        gtk_builder_add_from_file(p_builder, "views/Heure.glade", &p_err);

        if (p_err == NULL)
        {
            pthread_t thread;
            int rc = pthread_create(&thread, NULL, getTime, NULL);
            if (rc)
            {
                printf("ERROR; return code from pthread_create() is %d\n", rc);
                exit(-1);
            }
            /* Recuperation d'un pointeur sur la fenetre. */
            GObject *p_win = gtk_builder_get_object(p_builder, "MainWindow");
            gtk_window_fullscreen(GTK_WINDOW(p_win));
            g_signal_connect(p_win, "destroy", G_CALLBACK(cb_quit), NULL);
            set_navigation();
            set_alarm_buttons();
            gtk_widget_show_all(GTK_WIDGET(p_win));
            gtk_main();
        }
    }
}