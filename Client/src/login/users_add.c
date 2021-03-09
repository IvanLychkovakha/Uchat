#include "client.h"






void users_add(GtkToggleButton * button, t_uchat_bases * widgets){
    client->who_add = 1;
    guint16 text_l = gtk_entry_get_text_length (GTK_ENTRY(widgets->chat_g_name));
    int user_id = atoi(gtk_widget_get_name(GTK_WIDGET(button)));
    char * chs = (char*) gtk_entry_get_text (GTK_ENTRY(widgets->chat_g_name));
    if(text_l != 0){
        create_chat(client->socketfd,chs,client->current_user->id,0,"Resources/images/us_128.png.jpg",&user_id,1);
        g_print("");
        while (client->status == 0){
        g_print("");
        }
        client->status = 0;
        g_print("SUKA TEG ==  %d\n", user_id);
        create_user_button(&(widgets->us_data->user_but),widgets->grid_v);
        g_signal_connect(widgets->us_data->user_but->user_info[0],"toggled",G_CALLBACK(user_toggled),widgets);
        gtk_grid_attach(GTK_GRID(widgets->grid_user_online),widgets->us_data->user_but->user_info[0],0, widgets->grid_v,1,1);
        widgets->grid_v += 1;
        create_us_chat(widgets);
        gtk_widget_show(widgets->us_data->user_ch->bases);
    }
}
