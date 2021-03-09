#include "client.h"


static t_userbuton *add_element_start(t_userbuton *head, int j,int i){
    t_userbuton *tmp = g_slice_new(t_userbuton);
    tmp->user_info[0] = gtk_toggle_button_new ();
    gtk_widget_set_name(tmp->user_info[0],mx_itoa(client->chats[i]->id));
    //g_print(" id_chat== %s\n",mx_itoa(client->chats[i]->id));
    tmp->user_info[1] = gtk_grid_new ();
    gtk_container_add(GTK_CONTAINER(tmp->user_info[0]),tmp->user_info[1]);
    GdkPixbuf *    pixbuf;
    GError *error = 0;
    pixbuf = gdk_pixbuf_new_from_file_at_scale ("Resources/images/us_128.png",64,64,FALSE,&error);
    tmp->user_info[2] = gtk_image_new_from_pixbuf(pixbuf);
    tmp->user_info[3] = gtk_label_new (client->chats[i]->name);
    tmp->user_info[4] = gtk_label_new ("");
    tmp->user_info[5] = gtk_label_new ("");
    gtk_label_set_ellipsize (GTK_LABEL(tmp->user_info[5]),PANGO_ELLIPSIZE_END);
    tmp->user_info[6] = gtk_grid_new ();
    tmp->user_info[7] = gtk_grid_new ();
    gtk_grid_attach(GTK_GRID(tmp->user_info[1]),tmp->user_info[2],0,0,1,1);
    gtk_grid_attach(GTK_GRID(tmp->user_info[1]),tmp->user_info[6],1,0,1,1);
    gtk_grid_attach(GTK_GRID(tmp->user_info[6]),tmp->user_info[3],0,0,1,1);
    gtk_grid_attach(GTK_GRID(tmp->user_info[6]),tmp->user_info[4],1,0,1,1);
    gtk_grid_attach(GTK_GRID(tmp->user_info[6]),tmp->user_info[7],0,1,1,1);
    gtk_grid_attach(GTK_GRID(tmp->user_info[7]),tmp->user_info[5],0,0,1,1);
    gtk_widget_show_all(tmp->user_info[0]); 
    tmp -> next = head;
    return(tmp);
}


void  create_user_button(t_userbuton **t_user, int i){
  static int j = 0;
    if((*t_user) == NULL){
    t_userbuton * new = g_slice_new(t_userbuton);
    new->user_info[0] = gtk_toggle_button_new ();
    new->user_info[1] = gtk_grid_new ();
    gtk_widget_set_name(new->user_info[0],mx_itoa(client->chats[i]->id));
    //g_print(" id_chat== %s\n",mx_itoa(client->chats[i]->id) );
    gtk_container_add(GTK_CONTAINER(new->user_info[0]),new->user_info[1]);
    GdkPixbuf *    pixbuf;
    GError *error = 0;
    pixbuf = gdk_pixbuf_new_from_file_at_scale ("Resources/images/us_128.png",64,64,FALSE,&error);
    new->user_info[2] = gtk_image_new_from_pixbuf(pixbuf);
    //g_print("%s",client->chats[i]->name);
    //g_print("%s \n",gtk_widget_get_name(GTK_WIDGET(new->user_info[0])));
    new->user_info[3] = gtk_label_new (client->chats[i]->name);
    new->user_info[4] = gtk_label_new ("");
    new->user_info[5] = gtk_label_new ("");
    gtk_label_set_ellipsize ( GTK_LABEL(new->user_info[5]),PANGO_ELLIPSIZE_END);
    new->user_info[6] = gtk_grid_new ();
    new->user_info[7] = gtk_grid_new ();
    new->next = NULL;
    gtk_grid_attach(GTK_GRID(new->user_info[1]),new->user_info[2],0,0,1,1);
    gtk_grid_attach(GTK_GRID(new->user_info[1]),new->user_info[6],1,0,1,1);
    gtk_grid_attach(GTK_GRID(new->user_info[6]),new->user_info[3],0,0,1,1);
    gtk_grid_attach(GTK_GRID(new->user_info[6]),new->user_info[4],1,0,1,1);
    gtk_grid_attach(GTK_GRID(new->user_info[6]),new->user_info[7],0,1,1,1);
    gtk_grid_attach(GTK_GRID(new->user_info[7]),new->user_info[5],0,0,1,1);
    gtk_widget_show_all(new->user_info[0]);
    j++;
    (*t_user) = new;
    }else{
      (*t_user) =  add_element_start((*t_user),j, i);
      j++;
    }
  
    
}
