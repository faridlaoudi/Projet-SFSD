#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include <dirent.h>

GtkWidget *window;
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

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkBuilder *builder = gtk_builder_new_from_file("glade_interface.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    screen = GTK_WIDGET(gtk_builder_get_object(builder, "screen"));
   
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
