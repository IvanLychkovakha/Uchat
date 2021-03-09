#include "client.h"



void dialog_cancel(GtkButton * button, wig_log *widgets){
    gtk_widget_hide(widgets->win_dialog);
    gtk_image_set_from_file (GTK_IMAGE(widgets->user_photos),"Resources/images/us_128.png");
    widgets->files = "Resources/images/us_128.png";
}

void dialog_ok(GtkButton * button,wig_log *widgets){
    gtk_widget_hide(widgets->win_dialog);
    widgets->loop = FALSE;
    g_print("client_STATUS == 111 %d\n",client->status);
    char * phone = (char * )gtk_entry_get_text(GTK_ENTRY(widgets->in_login));
    char * name_first = (char * )gtk_entry_get_text(GTK_ENTRY(widgets->ip_ent_first));
    char * name_last = (char * )gtk_entry_get_text(GTK_ENTRY(widgets->ip_ent_last));
    char * password = (char * )gtk_entry_get_text(GTK_ENTRY(widgets->ip_pasword));
    g_print("phone == %s, f == %s, l == %s , p == %s",phone,name_first,name_last,password );
    GtkWidget * rand;
    GtkWidget * togled;
    int leng = 0;
    for(int i = 0; i < widgets->col_vo; i++){
        togled = gtk_grid_get_child_at (GTK_GRID(widgets->grid_tags),0,i);
        if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(togled)) == TRUE){
            g_print("LENG == %d\n",leng);
            leng++;
        }
    }
    int tags_id[leng];
    for(int i = 0,j = 0 ; i < widgets->col_vo && j < leng; i++){
        togled = gtk_grid_get_child_at (GTK_GRID(widgets->grid_tags),0,i);
        if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(togled)) == TRUE){
            rand = gtk_grid_get_child_at (GTK_GRID(widgets->grid_tags),1,i);
            //g_print("i == %d, j == %d,name == %s\n",i,j,gtk_widget_get_name(rand));
            g_print("tags_id == %d\n",atoi(gtk_widget_get_name(rand)));
            tags_id[j] = atoi(gtk_widget_get_name(rand));
            j++;
        }
    }
    registr_user(client,client->socketfd, name_first,name_last,password,phone,(char*) widgets->files,tags_id,leng);
    while(client->status == 0){
        g_print("");
    }
    g_print("client_STATUS == %d\n",client->status);
    if(client->status == 1){
        widgets->loop = FALSE;
        gtk_main_quit();
        gtk_widget_destroy(widgets->log_win);
        g_slice_free(wig_log,widgets);
        client->status = 0;
    }else{
            gtk_entry_set_text(GTK_ENTRY(widgets->in_login), "");
            gtk_entry_set_text(GTK_ENTRY(widgets->ip_ent_first), "");
            gtk_entry_set_text(GTK_ENTRY(widgets->ip_ent_last), "");
            gtk_entry_set_text(GTK_ENTRY(widgets->ip_pasword), "");
            gtk_entry_set_text(GTK_ENTRY(widgets->ip_confirm), "");
            client->status = 0;
    }
}


void set_photos(GtkButton * button, wig_log *widgets){
    GtkWidget *dialog;
    GdkPixbuf *    pixbuf;
    GError *error = 0;
    dialog = gtk_file_chooser_dialog_new("Chosse a file", GTK_WINDOW(widgets->win_dialog), GTK_FILE_CHOOSER_ACTION_OPEN,("_Cancel"),GTK_RESPONSE_CANCEL,("_Open"),GTK_RESPONSE_ACCEPT,NULL);
    gtk_widget_show_all(dialog);
//  gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog),"/");
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), g_get_current_dir());
    gint resp = gtk_dialog_run(GTK_DIALOG(dialog));
    if(resp == GTK_RESPONSE_ACCEPT){
        widgets->files  =  gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));

        pixbuf = gdk_pixbuf_new_from_file_at_scale (widgets->files,128,128,FALSE,&error);
        if(pixbuf != NULL){
            gtk_image_set_from_pixbuf (GTK_IMAGE(widgets->user_photos),pixbuf);
        }else{
                ;
        }
    }
    gtk_widget_destroy(dialog);

}


static void load_css(void);
static void initilizate_widget(wig_log **widgets,GtkBuilder *builder);
static void add_icod_to_entry(GtkEntry * entry, char * file_path,gboolean);
/*нажатие кнопки sigin  */
void sig_in_clicked(GtkButton * button, wig_log *widgets){
    /* проверка статуса */
    int status = sign_check_correct(widgets->l_error,GTK_ENTRY(widgets->in_ent_login),GTK_ENTRY(widgets->in_ent_password));
    if(!status){
        char * login = (char *) gtk_entry_get_text(GTK_ENTRY(widgets->in_ent_login));
        char * pass = (char *) gtk_entry_get_text(GTK_ENTRY(widgets->in_ent_password));
        login_user(client->socketfd, login, pass);

        while(client->status == 0){
            g_print("");
        }
        if(client->status == -1) {
            client->status=0;
            gtk_entry_set_text(GTK_ENTRY(widgets->in_ent_login),"");
            gtk_entry_set_text(GTK_ENTRY(widgets->in_ent_password),"");
        }
        else{
            client->status = 0;
            widgets->loop = FALSE;
            gtk_main_quit();
            gtk_widget_destroy(widgets->log_win);
            g_slice_free(wig_log,widgets);
        }


    }

}
/*нажатие кнопки sigin*/
void sig_up_clicked(GtkButton * button, wig_log *widgets){
    g_print("EROOR\n");
        if(check_register(widgets)){
           gtk_widget_show(GTK_WIDGET(widgets->win_dialog));
            if(client->tags[0] == NULL){
            get_all_tags(client->socketfd);
            }
           while(client->status == 0){
               g_print("XUIIII\n");
           }
           client->status = 0;
           for(int i = 0; client->tags[i] != NULL ; i ++){
               GtkWidget * check = gtk_check_button_new ();
               GtkWidget *  label = gtk_label_new(client->tags[i]->name);
               gtk_widget_set_name(label,mx_itoa(client->tags[i]->id));
               gtk_grid_attach(GTK_GRID(widgets->grid_tags),check,0,i,1,1 );
               gtk_grid_attach(GTK_GRID(widgets->grid_tags),label,1,i,1,1 );
               gtk_widget_show(check);
               gtk_widget_show(label);
               gtk_widget_set_hexpand (check,TRUE);
                gtk_widget_set_halign(check,GTK_ALIGN_END);
                gtk_widget_set_hexpand (label,TRUE);
                gtk_widget_set_halign(label,GTK_ALIGN_START);
                widgets->col_vo ++;
           }
        }

}

void hide_press(GtkWidget * widget, GdkEvent * event, wig_log *widgets){
    /*очистка и удалить*/
    gtk_widget_hide(widgets->l_error);
    gtk_widget_hide(widgets->l_error_2);
}
void hide_release(GtkWidget * widget, GdkEvent * event, wig_log *widgets){
    /*очистка и удалить*/
    gtk_widget_hide(widgets->l_error);
    gtk_widget_hide(widgets->l_error_2);
}

/*смена картинки */
void ip_press(GtkEntry * entry){
    static gboolean status = FALSE;
    if(!status){
        add_icod_to_entry(entry,"Resources/images/hide.png",status);
        status = TRUE;
    }
    else{
        add_icod_to_entry(entry,"Resources/images/view.png",status);
        status = FALSE;
    }
}
/*смена картинки */
void ip_en_icon_confirm(GtkEntry * entry){
    static gboolean status = FALSE;
    if(!status){
        add_icod_to_entry(entry,"Resources/images/hide.png",status);
        status = TRUE;
    }
    else{
        add_icod_to_entry(entry,"Resources/images/view.png",status);
        status = FALSE;
    }
}
/*смена картинки */
void in_ent_icon_password(GtkEntry * entry){
    static gboolean status = FALSE;
    if(!status){
        add_icod_to_entry(entry,"Resources/images/hide.png",status);
        status = TRUE;
    }
    else{
        add_icod_to_entry(entry,"Resources/images/view.png",status);
        status = FALSE;
    }
}


void log_main_loop(){
    gtk_init(NULL,NULL);
    load_css();
    wig_log *widgets = g_slice_new(wig_log);
    widgets->builder = gtk_builder_new_from_file("Resources/glade/log_reg.glade");
    widgets->log_win = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "log_win"));
    initilizate_widget(&widgets, widgets->builder);
    widgets->loop = FALSE;
    gtk_builder_connect_signals(widgets->builder, widgets);
    
    gtk_widget_show(widgets->log_win);
    gtk_main();
}


static void load_css(void){
    GtkCssProvider * provider;
    GdkDisplay * display;
    GdkScreen * screen;

    const gchar *css_style_file  = "Resources/css/login.css";
    GFile *css_fp = g_file_new_for_path(css_style_file);
 
    GError *error = 0;

    provider = gtk_css_provider_new();
    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen(display);

    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_css_provider_load_from_file(provider, css_fp, &error);

    g_object_unref(provider);
}

static void add_icod_to_entry(GtkEntry * entry, char * file_path,gboolean status){
    GdkPixbuf *    pixbuf;
    GError *error = 0;
    pixbuf = gdk_pixbuf_new_from_file (file_path,&error);
    gtk_entry_set_icon_from_pixbuf (entry,GTK_ENTRY_ICON_SECONDARY,pixbuf);
    if(!status){
        gtk_entry_set_visibility (entry,TRUE);
    }
    else{
        gtk_entry_set_visibility (entry,FALSE);
    }
}

static void initilizate_widget(wig_log **widgets,GtkBuilder *builder){
    (*widgets)->l_name = GTK_WIDGET(gtk_builder_get_object(builder, "l_name"));
    (*widgets)->in_ent_login = GTK_WIDGET(gtk_builder_get_object(builder, "in_ent_login"));
    (*widgets)->in_ent_password = GTK_WIDGET(gtk_builder_get_object(builder, "in_ent_password"));
    (*widgets)->l_error = GTK_WIDGET(gtk_builder_get_object(builder, "l_error"));
    (*widgets)->l_error_2 = GTK_WIDGET(gtk_builder_get_object(builder, "l_error_2"));
    (*widgets)->in_login = GTK_WIDGET(gtk_builder_get_object(builder, "in_login"));
    (*widgets)->ip_ent_last = GTK_WIDGET(gtk_builder_get_object(builder, "ip_ent_last"));
    (*widgets)->sig_in = GTK_WIDGET(gtk_builder_get_object(builder, "sig_in"));
    (*widgets)->step_two = GTK_WIDGET(gtk_builder_get_object(builder, "step_two"));
    (*widgets)->ip_ent_first = GTK_WIDGET(gtk_builder_get_object(builder, "ip_ent_first"));
    (*widgets)->ip_pasword = GTK_WIDGET(gtk_builder_get_object(builder, "ip_pasword"));
    (*widgets)->ip_confirm = GTK_WIDGET(gtk_builder_get_object(builder, "ip_confirm"));
    (*widgets)->c_1 = GTK_WIDGET(gtk_builder_get_object(builder, "c_1"));
    /* */
    (*widgets)->win_dialog = GTK_WIDGET(gtk_builder_get_object(builder, "win_dialog"));
    (*widgets)->us_add_photo = GTK_WIDGET(gtk_builder_get_object(builder, "us_add_photo"));
    (*widgets)->user_photos = GTK_WIDGET(gtk_builder_get_object(builder, "user_photos"));
    (*widgets)->grid_tags = GTK_WIDGET(gtk_builder_get_object(builder, "grid_tags"));
    (*widgets)->col_vo = 0;
     (*widgets)->files = "Resources/images/us_128.png";
    /* */
    gtk_entry_set_visibility (GTK_ENTRY((*widgets)->in_ent_password),FALSE);
    gtk_entry_set_invisible_char (GTK_ENTRY((*widgets)->in_ent_password),'*');
    gtk_entry_set_visibility (GTK_ENTRY((*widgets)->ip_pasword),FALSE);
    gtk_entry_set_invisible_char (GTK_ENTRY((*widgets)->ip_pasword),'*');
    gtk_entry_set_visibility (GTK_ENTRY((*widgets)->ip_confirm),FALSE);
    gtk_entry_set_invisible_char (GTK_ENTRY((*widgets)->ip_confirm),'*');
}



