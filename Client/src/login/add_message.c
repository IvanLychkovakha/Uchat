#include "client.h"



static t_user_base * search_user_chat(t_user_base * user_datas,const gchar * name){
    t_user_base * search = user_datas;
    if(search == NULL)
        return NULL;
    while(search->next != NULL){
        if(g_strcmp0(gtk_widget_get_name(search->bases),name) == 0){
            return search;
        }
        search = search->next;
    }
    if(g_strcmp0(gtk_widget_get_name(search->bases),name) == 0){
        return search;
    }
    return NULL;
}

static t_user_message  * push_massage(const gchar * label_text, t_user_message * head,t_user_base * base){
    t_user_message *tmp = g_slice_new(t_user_message);
    tmp->next = NULL;
    tmp->type_message = TRUE;
    tmp->text_message = gtk_label_new (label_text);
    tmp->grid = gtk_grid_new();
    GDateTime * times = g_date_time_new_now_local();
        
    tmp->time = gtk_label_new (g_date_time_format(times,"%H:%M"));
    gtk_widget_set_name(tmp->time,"label_time");
    gtk_grid_attach(GTK_GRID(tmp->grid),tmp->time, 0,0,1,1);
    gtk_grid_attach(GTK_GRID(tmp->grid),tmp->text_message, 0,1,1,1);
    gtk_widget_show_all(tmp->grid);
    gtk_widget_set_hexpand (tmp->grid,TRUE);
    gtk_widget_set_halign(tmp->grid,GTK_ALIGN_END);
    //
    gtk_widget_set_hexpand (tmp->time,TRUE);
    gtk_widget_set_halign(tmp->time,GTK_ALIGN_START);
    gtk_grid_set_row_spacing (GTK_GRID(tmp->grid),3);
    //
    gtk_grid_attach(GTK_GRID(base->bases), tmp->grid,0,base->leng_mes,1,1);
    base->leng_mes += 1;
    gtk_label_set_line_wrap(GTK_LABEL(tmp->text_message),TRUE);
    gtk_label_set_line_wrap_mode (GTK_LABEL(tmp->text_message), PANGO_WRAP_WORD_CHAR);
    gtk_label_set_max_width_chars(GTK_LABEL(tmp->text_message),60);
    
    tmp->next = head;
    return(tmp);
}


static void add_text_label(t_user_base * base,const gchar * label_text){
    if(base->messages == NULL){
        base->leng_mes = 0;
        base->messages = g_slice_new(t_user_message);
        base->messages->next = NULL;
        base->messages->type_message = TRUE;
        base->messages->text_message = gtk_label_new (label_text);
        base->messages->grid = gtk_grid_new();
        GDateTime * times = g_date_time_new_now_local();
        base->messages->time = gtk_label_new (g_date_time_format(times,"%H:%M"));
        gtk_widget_set_name(base->messages->time,"label_time");
        gtk_grid_attach(GTK_GRID(base->messages->grid),base->messages->time, 0,0,1,1);
        gtk_grid_attach(GTK_GRID(base->messages->grid),base->messages->text_message, 0,1,1,1);
        gtk_widget_show_all(base->messages->grid);
        gtk_widget_set_hexpand (base->messages->grid,TRUE);
        gtk_widget_set_halign(base->messages->grid,GTK_ALIGN_END);
        //
        gtk_widget_set_hexpand (base->messages->time,TRUE);
        gtk_widget_set_halign(base->messages->time,GTK_ALIGN_START);
        gtk_grid_set_row_spacing (GTK_GRID(base->messages->grid),3);
        //
        gtk_label_set_line_wrap(GTK_LABEL(base->messages->text_message),TRUE);
        gtk_label_set_line_wrap_mode (GTK_LABEL(base->messages->text_message), PANGO_WRAP_WORD_CHAR);
        gtk_label_set_max_width_chars (GTK_LABEL(base->messages->text_message),60);
        //
        //gtk_widget_set_hexpand ((base->messages->text_message),TRUE);
        //gtk_widget_set_halign((base->messages->text_message),GTK_ALIGN_END);  
        //      
        gtk_grid_attach(GTK_GRID(base->bases), base->messages->grid,0,base->leng_mes,1,1);
        base->leng_mes += 1;
        
    }else{
        base->messages = push_massage(label_text, base->messages, base);
    }
}

static t_user_message  * push_massage_another_user(const gchar * label_text, t_user_message * head,t_user_base * base){
    t_user_message *tmp = g_slice_new(t_user_message);
    tmp->next = NULL;
    tmp->type_message = TRUE;
    tmp->text_message = gtk_label_new (label_text);
    tmp->grid = gtk_grid_new();
    GDateTime * times = g_date_time_new_now_local();
    
    tmp->time = gtk_label_new (g_date_time_format(times,"%H:%M"));
    gtk_widget_set_name(tmp->time,"label_time");
    gtk_grid_attach(GTK_GRID(tmp->grid),tmp->time, 0,0,1,1);
    gtk_grid_attach(GTK_GRID(tmp->grid),tmp->text_message, 0,1,1,1);
    gtk_widget_show_all(tmp->grid);
    gtk_widget_set_hexpand(tmp->grid,TRUE);
    gtk_widget_set_halign(tmp->grid,GTK_ALIGN_START);
    //
    gtk_widget_set_hexpand (tmp->time,TRUE);
    gtk_widget_set_halign(tmp->time,GTK_ALIGN_START);
    gtk_grid_set_row_spacing (GTK_GRID(tmp->grid),3);
    //
    gtk_grid_attach(GTK_GRID(base->bases), tmp->grid,0,base->leng_mes,1,1);
    base->leng_mes += 1;
    gtk_label_set_line_wrap(GTK_LABEL(tmp->text_message),TRUE);
    gtk_label_set_line_wrap_mode (GTK_LABEL(tmp->text_message), PANGO_WRAP_WORD_CHAR);
    gtk_label_set_max_width_chars(GTK_LABEL(tmp->text_message),60);
    
    tmp->next = head;
    return(tmp);
}

static void add_text_label_another_user(t_user_base * base,const gchar * label_text){
    if(base->messages == NULL){
        base->leng_mes = 0;
        base->messages = g_slice_new(t_user_message);
        base->messages->next = NULL;
        base->messages->type_message = TRUE;
        base->messages->text_message = gtk_label_new (label_text);
        base->messages->grid = gtk_grid_new();
        GDateTime * times = g_date_time_new_now_local();
        base->messages->time = gtk_label_new (g_date_time_format(times,"%H:%M"));
        gtk_widget_set_name(base->messages->time,"label_time");
        gtk_grid_attach(GTK_GRID(base->messages->grid),base->messages->time, 0,0,1,1);
        gtk_grid_attach(GTK_GRID(base->messages->grid),base->messages->text_message, 0,1,1,1);
        gtk_widget_show_all(base->messages->grid);
        gtk_widget_set_hexpand (base->messages->grid,TRUE);
        gtk_widget_set_halign(base->messages->grid,GTK_ALIGN_START);
        //
        gtk_widget_set_hexpand (base->messages->time,TRUE);
        gtk_widget_set_halign(base->messages->time,GTK_ALIGN_START);
        gtk_grid_set_row_spacing (GTK_GRID(base->messages->grid),3);
        //
        gtk_label_set_line_wrap(GTK_LABEL(base->messages->text_message),TRUE);
        gtk_label_set_line_wrap_mode (GTK_LABEL(base->messages->text_message), PANGO_WRAP_WORD_CHAR);
        gtk_label_set_max_width_chars (GTK_LABEL(base->messages->text_message),60);
        //
        //gtk_widget_set_hexpand ((base->messages->text_message),TRUE);
        //gtk_widget_set_halign((base->messages->text_message),GTK_ALIGN_END);  
        //      
        gtk_grid_attach(GTK_GRID(base->bases), base->messages->grid,0,base->leng_mes,1,1);
        base->leng_mes += 1;
        
    }else{
        base->messages = push_massage_another_user(label_text, base->messages, base);
    }
}






void add_message(t_uchat_bases * widgest, int type, char * text_message, int chat){
    if(type == 1){
      //  g_print("TABLAT_SUKA\n");
        char * sup_text = (char *) gtk_widget_get_name(widgest->active_grid);
     //   g_print("TABLAT_SUKA\n");
        t_user_base * base = search_user_chat(widgest->us_data->user_ch,sup_text);
     //   g_print("TABLAT_SUKA\n");
        add_text_label(base,text_message);
       // g_print("TABLAT_SUKA\n");
        gtk_entry_set_text(GTK_ENTRY(widgest->chat_entry),"");
    }else {
        if(type == 2){
             char * sup_text = (char *) gtk_widget_get_name(widgest->active_grid);
            t_user_base * base = search_user_chat(widgest->us_data->user_ch,sup_text);
            add_text_label_another_user(base,text_message);
        }else{
            if(type == 3){
            char * sup_text = (char *) gtk_widget_get_name(widgest->active_grid);
            t_user_base * base = search_user_chat(widgest->us_data->user_ch,sup_text);
            //g_print("TEXT == %s, user_char %d\n" ,text_message, atoi(sup_text));
            add_text_label(base,text_message);
            send_message(client->socketfd,client->current_user->id,atoi(sup_text),text_message);
            gtk_entry_set_text(GTK_ENTRY(widgest->chat_entry),"");
            }else{
                t_user_base * base = search_user_chat(widgest->us_data->user_ch,mx_itoa(chat));
                add_text_label_another_user(base,text_message);
            }
        }
    }


}





