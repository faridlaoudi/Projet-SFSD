#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include <dirent.h>

GtkWidget *window;
GtkWidget *dialog;
GtkWidget *screen;
GtkWidget *fixed1;
GtkWidget *fixed2;
GtkButton *button1;
GtkButton *create;
GtkButton *insert;
GtkButton *delete;
GtkButton *search;
GtkWidget *signature;
GtkLabel *title;
GtkWidget *logo;

void on_button1_clicked(GtkButton *button, gpointer user_data){
    g_print("Button clicked\n");  // Debug print
    gtk_widget_show_all(screen);
    gtk_widget_hide(window);
}
void on_create_clicked(GtkButton *button, gpointer user_data) {
    
    gtk_widget_show_all(dialog);

    gint response = gtk_dialog_run(GTK_DIALOG(dialog));

    // Handle the response (e.g., 'OK' or 'Cancel' button clicked)
    if (response == GTK_RESPONSE_OK) {
        // Get the text from the GtkEntry
        void on_entry1_changed(GtkWidget *w) {
            printf("%s\n", "on.entry1.changed.h");
            printf("\t%s\n", gtk_entry_get_text(GTK_ENTRY(w)));
        }
        void on_entry2_changed(GtkWidget *e) {
            printf("%s\n", "on.entry2.changed.h");
            printf("\t%s\n", gtk_entry_get_text(GTK_ENTRY(e)));
        }

    }

    // Hide the dialog
    gtk_widget_hide(dialog);
}


int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkBuilder *builder = gtk_builder_new_from_file("glade_interface.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    screen = GTK_WIDGET(gtk_builder_get_object(builder, "screen"));
    dialog = GTK_WIDGET(gtk_builder_get_object(builder, "dialog"));
    GtkWidget *dialog1 = GTK_WIDGET(gtk_builder_get_object(builder, "dialog1"));
    GtkLabel *filename = GTK_LABEL(gtk_builder_get_object(builder, "filename"));
    GtkLabel *nbeng = GTK_LABEL(gtk_builder_get_object(builder, "nbeng"));
    GtkWidget *check = GTK_WIDGET(gtk_builder_get_object(builder, "check"));
    GtkWidget *cancel = GTK_WIDGET(gtk_builder_get_object(builder, "cancel"));
    GtkWidget *ok = GTK_WIDGET(gtk_builder_get_object(builder, "ok"));
    GtkWidget *entry1 = GTK_WIDGET(gtk_builder_get_object(builder, "entry1"));
    GtkWidget *entry2 = GTK_WIDGET(gtk_builder_get_object(builder, "entry2"));
   
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(screen, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    
    gtk_builder_connect_signals(builder, NULL);

    fixed1 = GTK_WIDGET(gtk_builder_get_object(builder, "fixed1"));
    fixed2 = GTK_WIDGET(gtk_builder_get_object(builder, "fixed2"));
    button1 = GTK_BUTTON(gtk_builder_get_object(builder, "button1"));
    create = GTK_BUTTON(gtk_builder_get_object(builder, "create"));
    insert = GTK_BUTTON(gtk_builder_get_object(builder, "insert"));
    delete = GTK_BUTTON(gtk_builder_get_object(builder, "delete"));
    search = GTK_BUTTON(gtk_builder_get_object(builder, "search"));
    signature = GTK_WIDGET(gtk_builder_get_object(builder, "signature"));
    logo = GTK_WIDGET(gtk_builder_get_object(builder, "logo"));
    title = GTK_LABEL(gtk_builder_get_object(builder, "title"));
    g_signal_connect(button1, "clicked", G_CALLBACK(on_button1_clicked), NULL);

    
    gtk_widget_show_all(window);
    gtk_main();
    return EXIT_SUCCESS;
}
