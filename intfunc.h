#ifndef INTFUNC_H
#define INTFUNC_H
#include"functions.h"
#include <dirent.h>
const gchar *text1 = " ";
const gchar *infoeng1 = " ";
int nbg;
Enreg E;
E.sup=0;
//main screen

void on_search_clicked(GtkButton *button, gpointer user_data) {
    g_print("Button clicked\n");
    gtk_label_set_text (GTK_LABEL(files1), "hi");
}
void on_button1_clicked(GtkButton *button, gpointer user_data){
    g_print("Button clicked\n");  // Debug print
    gtk_widget_show_all(screen);
    gtk_widget_hide(window);
    char *text = g_strdup(" ");
    gchar *old_text;
    int i=1;
    struct dirent *entry;
    DIR *dir = opendir(".");

    if (dir == NULL) {
        perror("Error opening directory");
        return;
    }

    printf("avialable files :\n");
    while ((entry = readdir(dir)) != NULL) {
        if (hasExtension(entry->d_name, ".txt")) {
            char index[10];
            sprintf(index, "%d- ", i); // Convert integer to string
            old_text = text;
            text = g_strconcat(text, index, entry->d_name, "\n", NULL);
            g_free(old_text);
            i++;
        }
    }
    gtk_label_set_text (GTK_LABEL(files1), text);
    closedir(dir);
    
}
void on_create_clicked(GtkButton *button, gpointer user_data) {
    gtk_widget_show_all(dialog);
    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
}

//view Data

void on_view_clicked(GtkButton *button, gpointer user_data){
        g_print("view\n");  // Debug print
        char *text2 = g_strdup(" ");  // Allocate memory
        gchar *old_text2;
        char *text3 = g_strdup(" ");
        char *text4 = g_strdup(" ");
        TOVC *filepointer = ouvrir(text1,'A');
        int k=1,i1=1,l=0,j1=0;
        Enreg E;
        semi_enreg SE;
        sprintf(text2, "ENTETE : %d   %d   %d   %d\n",entete(filepointer,1),entete(filepointer,2),entete(filepointer,3),entete(filepointer,4));
        while (k<=entete(filepointer,1))
        {
            recupsemi_enreg(filepointer,SE,&i1,&j1);
            SemitoEnreg(SE,&E);
            sprintf(text3,"%d|%d|%s",E.cle,E.sup,E.info);
            if (k==i1) {sprintf(text4," Dans le Bloc %d\n",k);}
            else {sprintf(text4," commence du bloc %d et chevauche le bloc %d\n",k,i1);}
            if (j1==Taille_Bloc) {i1++;j1=0;}
            k=i1;l=j1;
            if ((k==entete(filepointer,1)) && l==entete(filepointer,3)) {break;}
            old_text2 = text2;
            text2 = g_strconcat(text2, text3, text4, NULL);
            g_free(old_text2);
        }
        gtk_label_set_text (GTK_LABEL(affichage), text2);
    }

 //create and open files

 void on_ok_clicked(GtkButton *button, gpointer user_data){
        g_print("ok\n");  // Debug print
        gtk_widget_show_all(screen);
        gtk_widget_hide(dialog);
        TOVC *filepointer = ouvrir(text1,'N');
        gtk_label_set_text (GTK_LABEL(files), "");
        gtk_label_set_text (GTK_LABEL(files1), text1);
    }
void on_cancel_clicked(GtkButton *button, gpointer user_data){
    g_print("cancel\n");  // Debug print
    gtk_widget_show_all(screen);
    gtk_widget_hide(dialog);
}

void on_check_toggled(GtkWidget *w) {
    int j=0;
    printf("%s\n", "on.SampleGtkCheckButton.toggled.h");
    j=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(w));
    printf("\tchecked = %d\n", j);
    
 }
void on_entry1_changed(GtkWidget *w) {
    text1 =gtk_entry_get_text(GTK_ENTRY(w));
 }


//insertion button
void on_insert_clicked(GtkButton *button, gpointer user_data) {
    gtk_widget_show_all(dialoginsert);
    gint response = gtk_dialog_run(GTK_DIALOG(dialoginsert));
}
void on_cleeeng_value_changed(GtkWidget *w) {
 printf("%s\n", "on.SampleGtkSpinButton.value.changed.h");
 printf("spin = %d\n", (int) gtk_spin_button_get_value(GTK_SPIN_BUTTON(w)));
 nbg = (int) gtk_spin_button_get_value(GTK_SPIN_BUTTON(w));
 }
void on_info_changed(GtkWidget *w) {
    infoeng1 = gtk_entry_get_text(GTK_ENTRY(w));
    printf("%s %s\n", "Info :",infoeng1);
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
    E.cle = nbg;
    strcpy(E.info, infoeng1);
    insertion_TOVC(filepointer, E);
}



#endif