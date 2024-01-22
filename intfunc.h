#ifndef INTFUNC_H
#define INTFUNC_H

void on_button1_clicked(GtkButton *button, gpointer user_data){
    g_print("Button clicked\n");  // Debug print
    gtk_widget_show_all(screen);
    gtk_widget_hide(window);
}
void on_create_clicked(GtkButton *button, gpointer user_data) {
    gtk_widget_show_all(dialog);
    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
}
void on_ok_clicked(GtkButton *button, gpointer user_data){
    g_print("ok\n");  // Debug print
    gtk_widget_show_all(screen);
    gtk_widget_hide(dialog);
}
void on_cancel_clicked(GtkButton *button, gpointer user_data){
    g_print("cancel\n");  // Debug print
    gtk_widget_show_all(screen);
    gtk_widget_hide(dialog);
}
void on_check_toggled(GtkWidget *w) {
 printf("%s\n", "on.SampleGtkCheckButton.toggled.h");
 printf("\tchecked = %d\n", gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(w)));
 }
void on_entry1_changed(GtkWidget *w) {
 printf("%s\n", "Name is :");
 printf("\t%s\n", gtk_entry_get_text(GTK_ENTRY(w)));
 }




//insertion button
void on_insert_clicked(GtkButton *button, gpointer user_data) {
    gtk_widget_show_all(dialoginsert);
    gint response = gtk_dialog_run(GTK_DIALOG(dialoginsert));
}
void on_info_changed(GtkWidget *w) {
 printf("%s\n", "Info :");
 printf("\t%s\n", gtk_entry_get_text(GTK_ENTRY(w)));
 }
void on_cancelinsert_clicked(GtkButton *button, gpointer user_data){
    g_print("cancel\n");  // Debug print
    gtk_widget_show_all(screen);
    gtk_widget_hide(dialoginsert);
}
void on_okinsert_clicked(GtkButton *button, gpointer user_data){
    g_print("ok\n");  // Debug print
    gtk_widget_show_all(screen);
    gtk_widget_hide(dialoginsert);
}
void on_cleeeng_value_changed(GtkWidget *w) {
 printf("%s\n", "key is :");
 printf("spin = %d\n", (int) gtk_spin_button_get_value(GTK_SPIN_BUTTON(w)));
 }


#endif