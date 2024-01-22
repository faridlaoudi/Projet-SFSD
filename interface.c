#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "intdecl.h"
#include "intfunc.h"

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkBuilder *builder = gtk_builder_new_from_file("glade_interface.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    screen = GTK_WIDGET(gtk_builder_get_object(builder, "screen"));
    dialog = GTK_WIDGET(gtk_builder_get_object(builder, "dialog"));
    dialog1 = GTK_WIDGET(gtk_builder_get_object(builder, "dialog1"));
    dialoginsert = GTK_WIDGET(gtk_builder_get_object(builder, "dialoginsert"));
    dialoginsert1 = GTK_WIDGET(gtk_builder_get_object(builder, "dialoginsert1"));
    infoeng = GTK_LABEL(gtk_builder_get_object(builder, "infoeng"));
    filename = GTK_LABEL(gtk_builder_get_object(builder, "filename"));
    clee = GTK_LABEL(gtk_builder_get_object(builder, "clee"));
    infoeng = GTK_LABEL(gtk_builder_get_object(builder, "infoeng"));
    check = GTK_WIDGET(gtk_builder_get_object(builder, "check"));
    cancel = GTK_WIDGET(gtk_builder_get_object(builder, "cancel"));
    ok = GTK_WIDGET(gtk_builder_get_object(builder, "ok"));
    entry1 = GTK_WIDGET(gtk_builder_get_object(builder, "entry1"));
    cleeeng = GTK_WIDGET(gtk_builder_get_object(builder, "cleeng"));
    info = GTK_WIDGET(gtk_builder_get_object(builder, "info"));
    lb = GTK_LABEL(gtk_builder_get_object(builder, "lb"));
   
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(screen, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    
    gtk_builder_connect_signals(builder, NULL);

    fixed1 = GTK_WIDGET(gtk_builder_get_object(builder, "fixed1"));
    fixed2 = GTK_WIDGET(gtk_builder_get_object(builder, "fixed2"));
    button1 = GTK_BUTTON(gtk_builder_get_object(builder, "button1"));
    view = GTK_WIDGET(gtk_builder_get_object(builder, "view"));
    create = GTK_BUTTON(gtk_builder_get_object(builder, "create"));
    okinsert = GTK_WIDGET(gtk_builder_get_object(builder, "okinsert"));
    cancelinsert = GTK_WIDGET(gtk_builder_get_object(builder, "cancelinsert"));
    insert = GTK_BUTTON(gtk_builder_get_object(builder, "insert"));
    delete = GTK_BUTTON(gtk_builder_get_object(builder, "delete"));
    search = GTK_WIDGET(gtk_builder_get_object(builder, "search"));
    signature = GTK_WIDGET(gtk_builder_get_object(builder, "signature"));
    logo = GTK_WIDGET(gtk_builder_get_object(builder, "logo"));
    affichage = GTK_WIDGET(gtk_builder_get_object(builder, "affichage"));
    title = GTK_LABEL(gtk_builder_get_object(builder, "title"));
    files = GTK_LABEL(gtk_builder_get_object(builder, "files"));
    files1 = GTK_WIDGET(gtk_builder_get_object(builder, "files1"));
    g_signal_connect(button1, "clicked", G_CALLBACK(on_button1_clicked), NULL);
    g_signal_connect(search, "clicked", G_CALLBACK(on_search_clicked), NULL);
    
    gtk_widget_show_all(window);
    gtk_main();
    return EXIT_SUCCESS;
}
