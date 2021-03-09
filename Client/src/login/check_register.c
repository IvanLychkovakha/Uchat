#include "client.h"



static int check_pass(guint16 leng_p,guint16 leng_c, const gchar * login,const gchar * password ){
    if(leng_p == 0 && leng_p < 1){
        return ER_PASSWORD_LENG;
    }
     if(leng_c == 0 && leng_c < 1 && g_strcmp0(login,password) != 0){
         return ER_PASSWORD_LENG;
    }
    return 0;
}

static int check_login_with_phone(guint16 leng_l,guint16 leng_f,guint16 leng_la ){
     if(leng_l == 0 && leng_l > 15){
         return ER_LOGIN_LENG;
    }
    if(leng_f == 0){
        return ER_LOGIN_LENG;
    }
    if(leng_la == 0){
        return ER_LOGIN_LENG;
    }
    return 0;
}

static int check_login(guint16 leng_f,guint16 leng_la){
    if(leng_f == 0){
        return ER_LOGIN_LENG;
    }
    if(leng_la == 0){
        return ER_LOGIN_LENG;
    }
    return 0;
}

int check_register(wig_log *widgets){
        guint16 leng_l =   gtk_entry_get_text_length (GTK_ENTRY(widgets->in_login));
        guint16 leng_f =   gtk_entry_get_text_length (GTK_ENTRY(widgets->ip_ent_first));
        guint16 leng_la =  gtk_entry_get_text_length (GTK_ENTRY(widgets->ip_ent_last));
        guint16 leng_p =   gtk_entry_get_text_length (GTK_ENTRY(widgets->ip_pasword));
        guint16 leng_c =   gtk_entry_get_text_length (GTK_ENTRY(widgets->ip_confirm));
        if(check_pass(leng_p,leng_c,gtk_entry_get_text(GTK_ENTRY(widgets->ip_pasword)),gtk_entry_get_text(GTK_ENTRY(widgets->ip_confirm))) == ER_PASSWORD_LENG  &&check_login_with_phone(leng_l,leng_f,leng_la)){
            gtk_widget_show(widgets->l_error_2);
            gtk_entry_set_text(GTK_ENTRY(widgets->in_login), "");
            gtk_entry_set_text(GTK_ENTRY(widgets->ip_ent_first), "");
            gtk_entry_set_text(GTK_ENTRY(widgets->ip_ent_last), "");
            gtk_entry_set_text(GTK_ENTRY(widgets->ip_pasword), "");
            gtk_entry_set_text(GTK_ENTRY(widgets->ip_confirm), "");
            return 0;
        }
        return 1;
}

