#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include <math.h>
#include <glib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#define MAX_BUFFER_SIZE 1024

GtkBuilder *builder;
GtkWidget *window;
GtkWidget *hideMi;
GtkWidget *pmenu;
GtkListStore *liststore;
GtkCellRenderer *ccl1, *ccl2, *ccl3, *ccl4, *ccl5;

void populate_list_store(GtkListStore *liststore, GtkTreeIter *iter) {
    FILE *fp;
    char path[1035];
    /* Open the command for reading. */
    fp = popen("ps -eo user,pid,pmem,pcpu,comm,cmd --sort +pid", "r");
    if (fp == NULL) {
        printf("Failed to run command\n" );
        exit(1);
    }
    /* Read the output a line at a time - output it. */
    int i = 0;
    int lastpid = 0;
    while (fgets(path, sizeof(path), fp) != NULL) {
        char user[500], comm[500], cmd[500];
        int pid;
        float pmem, pcpu;
        sscanf(path, "%s %d %f %f %s %s", user, &pid, &pmem, &pcpu, comm, cmd);
        if(i==0|| lastpid == pid){
            i++;
            continue;
        }
        char *pmem_str = g_strdup_printf("%.1f", pmem);
        char *pcpu_str = g_strdup_printf("%.1f", pcpu); 
        gtk_list_store_append(liststore, iter);
        gtk_list_store_set(liststore, iter, 0, user, 1, comm, 3, pmem_str, 4, pcpu_str, 2, pid, -1);
        g_free(pmem_str);
        g_free(pcpu_str);
        lastpid = pid;
    }
    /* close */
    pclose(fp);
}
gboolean refresh() {
    GtkTreeIter iter;
    GtkListStore *liststore;
    liststore = GTK_LIST_STORE(gtk_builder_get_object(builder, "liststore1"));
    gtk_list_store_clear(liststore);
    populate_list_store(liststore, &iter);
    return TRUE; // Return TRUE to be called again
}
int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("test4.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

    gtk_builder_connect_signals(builder, NULL);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    
    ccl1 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "ccl1"));
    ccl2 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "ccl2"));
    ccl3 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "ccl3"));
    ccl4 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "ccl4"));
    ccl5 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "ccl5"));
    hideMi = GTK_WIDGET(gtk_builder_get_object(builder, "hideMi"));
    pmenu = GTK_WIDGET(gtk_builder_get_object(builder, "pmenu"));
// Add first row
// func(liststore, iter, 0, "1");
// func2(liststore, &iter, 0, func, "pid", "+");
// func2(liststore, &iter, 1, func, "pmem", "+");
// func2(liststore, &iter, 2, func, "pcpu", "+");
    // GtkListStore *liststore = gtk_list_store_new(5, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT);



    GtkTreeIter iter;
    liststore = GTK_LIST_STORE(gtk_builder_get_object(builder, "liststore1"));
    populate_list_store(liststore, &iter);
    g_object_set(ccl1, "xalign", 0.5, NULL); 
    g_object_set(ccl2, "xalign", 0.5, NULL); 
    g_object_set(ccl3, "xalign", 0.5, NULL); 
    g_object_set(ccl4, "xalign", 0.5, NULL); 
    g_object_set(ccl5, "xalign", 0.5, NULL); 
// // Add second row
// gtk_list_store_append(liststore, &iter);
// gtk_list_store_set(liststore, &iter, 0, "2", 1, "d", -1);

// // Add third row
// gtk_list_store_append(liststore, &iter);
// gtk_list_store_set(liststore, &iter, 0, "3", 1, "c", -1);
    // g_signal_connect_swapped(, "button-press-event", G_CALLBACK(show_popup), pmenu);  
    // gtk_widget_show(hideMi);

    
    // g_timeout_add_seconds(5, (GSourceFunc)refresh, NULL);
    gtk_widget_show(window);
    gtk_main();

    return 0;
}

void show_popup(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    if (event->button == 3) // Right mouse button
    {
        gtk_menu_popup_at_pointer(GTK_MENU(widget), (GdkEvent*)event);
    }
}
void sendsignal(gint *value3,const gchar *key){
    char value3tostring[50];
    char value[50];
    if(strcmp(key, "start") == 0){
        sprintf(value3tostring, "kill -18 %d", value3);

    }
    else if (strcmp(key, "end_task") == 0)
    {
        sprintf(value3tostring, "kill -15 %d", value3);
    }else if (strcmp(key, "stop") == 0)
    {
        sprintf(value3tostring, "kill -19 %d", value3);
    }
    else if(strcmp(key, "SIGKILL") == 0){
        sprintf(value3tostring, "kill -9 %d", value3);
    }
    else if(strcmp(key, "SIGTERM") == 0){
        sprintf(value3tostring, "kill -15 %d", value3);
    }
    else if(strcmp(key, "SIGTSTP") == 0){
        sprintf(value3tostring, "kill -20 %d", value3);
    }
    else if(strcmp(key, "SIGCONT") == 0){
        sprintf(value3tostring, "kill -18 %d", value3);
    }
    else if(strcmp(key, "SIGINT") == 0){
        sprintf(value3tostring, "kill -2 %d", value3);
    }
    else if(strcmp(key, "SIGHUP") == 0){
        sprintf(value3tostring, "kill -1 %d", value3);
    }
    else if(strcmp(key, "SIGUSR1") == 0){
        sprintf(value3tostring, "kill -10 %d", value3);
    }
    else if(strcmp(key, "SIGUSR2") == 0){
        sprintf(value3tostring, "kill -12 %d", value3);
    }
    // printf("%s\n", value3tostring);
    system(value3tostring);
    refresh();
}
void refresh_btn(GtkWidget *widget, GdkEventButton *event){
    refresh();
}
void print_data(GtkWidget *widget, GdkEventButton *event, const gchar *key){
    gint *value3 = g_object_get_data(G_OBJECT(widget), key);
    if (value3) 
        sendsignal(value3, key);
}

void start(GtkWidget *widget, GdkEventButton *event){
    print_data(widget, event, "start");
}
void end_task(GtkWidget *widget, GdkEventButton *event){
    print_data(widget, event, "end_task");
}
void stop(GtkWidget *widget, GdkEventButton *event){
    print_data(widget, event, "stop");
}
void sigkill(GtkWidget *widget, GdkEventButton *event){
    print_data(widget, event, "SIGKILL");
}
void sigterm(GtkWidget *widget, GdkEventButton *event){
    print_data(widget, event, "SIGTERM");
}
void sigtstp(GtkWidget *widget, GdkEventButton *event){
    print_data(widget, event, "SIGTSTP");
}
void sigcont(GtkWidget *widget, GdkEventButton *event){
    print_data(widget, event, "SIGCONT");
}
void sigint(GtkWidget *widget, GdkEventButton *event){
    print_data(widget, event, "SIGINT");
}
void sighup(GtkWidget *widget, GdkEventButton *event){
    print_data(widget, event, "SIGHUP");
}
void sigusr1(GtkWidget *widget, GdkEventButton *event){
    print_data(widget, event, "SIGUSR1");
}
void sigusr2(GtkWidget *widget, GdkEventButton *event){
    print_data(widget, event, "SIGUSR2");
}
void selected_changed_cb(GtkTreeSelection *tc, gpointer user_data) {
    gint *value3;
    GtkTreeIter iter;
    GtkTreeModel *model;
    if (gtk_tree_selection_get_selected(tc, &model, &iter)){
        gtk_tree_model_get(model, &iter, 2, &value3, -1);
        if (user_data ) {
            g_object_set_data(G_OBJECT(user_data), "start", value3);
            g_object_set_data(G_OBJECT(user_data), "end_task", value3);
            g_object_set_data(G_OBJECT(user_data), "stop", value3);
            g_object_set_data(G_OBJECT(user_data), "SIGKILL", value3);
            g_object_set_data(G_OBJECT(user_data), "SIGTERM", value3);
            g_object_set_data(G_OBJECT(user_data), "SIGTSTP", value3);
            g_object_set_data(G_OBJECT(user_data), "SIGCONT", value3);
            g_object_set_data(G_OBJECT(user_data), "SIGINT", value3);
            g_object_set_data(G_OBJECT(user_data), "SIGHUP", value3);
            g_object_set_data(G_OBJECT(user_data), "SIGUSR1", value3);
            g_object_set_data(G_OBJECT(user_data), "SIGUSR2", value3);
        } 
    }
}
