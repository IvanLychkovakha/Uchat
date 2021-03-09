#include "client.h"



static t_user_search  * add_to_search_list(t_user_search  * search, int i, int v_g){
   t_user_search * new_s = g_slice_new(t_user_search);
    new_s->togle_but = gtk_toggle_button_new ();
    new_s->g_base = gtk_grid_new();
    GdkPixbuf *    pixbuf;
    GError *error = 0;
    pixbuf = gdk_pixbuf_new_from_file_at_scale (client->searced_users[i]->image,64,64,FALSE,&error);
    new_s->user_image  =  gtk_image_new_from_pixbuf(pixbuf);
    gtk_container_add(GTK_CONTAINER(new_s->togle_but),new_s->g_base);
    g_signal_connect(new_s->togle_but,"toggled",G_CALLBACK(users_add),widgets);
    gchar * text = mx_strjoin(client->searced_users[i]->name, " ");
    new_s->label_name = gtk_label_new(mx_strjoin(text,client->searced_users[i]->lastname));
    gtk_grid_attach(GTK_GRID(new_s->g_base),new_s->user_image,0,v_g,1,1);
    gtk_grid_attach(GTK_GRID(new_s->g_base),new_s->label_name,1,v_g,1,1);
    gtk_widget_set_name(new_s->togle_but, mx_itoa(client->searced_users[i]->id));
    gtk_widget_show_all(new_s->togle_but);
    gtk_grid_attach(GTK_GRID(widgets->grid_popov_users),new_s->togle_but, 0,v_g,1,1);
    new_s -> next = search;
    return(new_s);
}



void search_list(t_user_search  ** search, int i){
    static int v_g = 0;
    if((*search) == NULL){
        (*search) = g_slice_new(t_user_search);
        (*search)->togle_but = gtk_toggle_button_new ();
        (*search)->g_base = gtk_grid_new();
        GdkPixbuf *    pixbuf;
        GError *error = 0;
        pixbuf = gdk_pixbuf_new_from_file_at_scale (client->searced_users[i]->image,64,64,FALSE,&error);
        (*search)->user_image  =  gtk_image_new_from_pixbuf(pixbuf);
        gtk_container_add(GTK_CONTAINER((*search)->togle_but),(*search)->g_base);
        g_signal_connect((*search)->togle_but,"toggled",G_CALLBACK(users_add),widgets);
        gchar * text = mx_strjoin(client->searced_users[i]->name, " ");
        (*search)->label_name = gtk_label_new(mx_strjoin(text,client->searced_users[i]->lastname));
        gtk_grid_attach(GTK_GRID((*search)->g_base),(*search)->user_image,0,0,1,1);
        gtk_grid_attach(GTK_GRID((*search)->g_base),(*search)->label_name,1,0,1,1);
        gtk_widget_set_name((*search)->togle_but, mx_itoa(client->searced_users[i]->id));
        gtk_widget_show_all((*search)->togle_but);
        gtk_grid_attach(GTK_GRID(widgets->grid_popov_users),(*search)->togle_but, 0,0,1,1);
        (*search)->next = NULL;
        v_g++;
    }else{
        (*search) = add_to_search_list((*search),i,v_g);
        v_g++;
    }
}

