#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include <dirent.h>

GtkWidget *window;
GtkWidget *fixed1;
GtkButton *button1;
GtkWidget *signature;
GtkLabel *title;
GtkWidget *logo;

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkBuilder *builder = gtk_builder_new_from_file("glade_interface.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_builder_connect_signals(builder, NULL);

    fixed1 = GTK_WIDGET(gtk_builder_get_object(builder, "fixed1"));
    button1 = GTK_BUTTON(gtk_builder_get_object(builder, "button1"));
    signature = GTK_WIDGET(gtk_builder_get_object(builder, "signature"));
    logo = GTK_WIDGET(gtk_builder_get_object(builder, "logo"));
    title = GTK_LABEL(gtk_builder_get_object(builder, "title"));

    
    gtk_widget_show(window);
    gtk_main();
    return EXIT_SUCCESS;
}

void on_button1_clicked(GtkButton *button, gpointer user_data){
    g_print("Button clicked\n");  // Debug print
    gtk_label_set_text(GTK_LABEL(title), "Welcome");
}
