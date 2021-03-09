#include "client.h"

static int check_leng(guint16 leng_l,guint16 leng_p){
    if(leng_l == 0 && leng_p == 0)
        return ER_LOGIN_PASSWORD;
    if(leng_l == 0)
        return ER_LOGIN_0;
    if(leng_p == 0)
        return ER_PASSWORD_0;
    if(leng_l > 14)
        return ER_LOGIN_0; 
    if(leng_p < 1)
        return ER_PASSWORD_LENG;
    return OK;
}

static void messege_error(GtkWidget * er_label,GtkEntry * login, GtkEntry * password){
    gtk_entry_set_text(login, ""); 
    gtk_entry_set_text(password, "");
    gtk_widget_show(er_label);
}


int sign_check_correct(GtkWidget * er_label,GtkEntry * login, GtkEntry * password){
    guint16 leng_l =   gtk_entry_get_text_length (login);
    guint16 leng_p =   gtk_entry_get_text_length (password);
    int status = check_leng(leng_l,leng_p);
    if(status == OK){
        return 0;
    }
    messege_error( er_label,  login, password);
    return 1;
}
