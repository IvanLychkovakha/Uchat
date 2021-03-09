#pragma once

#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#define NORMAL_LOOP 0
#define WINDOW_DESTROY 1

enum entry_leng_er{ER_LOGIN_PASSWORD,
    ER_LOGIN_0,
    ER_PASSWORD_0,
    ER_LOGIN_LENG,
    ER_PASSWORD_LENG,
    OK
    };

typedef struct wig_log wig_log;

struct wig_log{
    GtkWidget *l_name;
    GtkWidget *stack_log;
    GtkWidget *in_ent_login;
    GtkWidget *in_ent_password;
    GtkWidget *l_error;
    GtkWidget *in_login;
    GtkWidget *ip_ent_last;
    GtkWidget *sig_in;
    GtkWidget *step_two;
    GtkWidget *ip_ent_first;
    GtkWidget *ip_pasword;
    GtkWidget *ip_confirm;
    GtkWidget *c_1;
    GtkWidget *l_error_2;
    gboolean loop;
    GtkWidget * win_dialog;
    GtkWidget * us_add_photo;
    GtkWidget * user_photos;
    GtkWidget * grid_tags;
    gint col_vo;
    gchar * files;
    GtkBuilder *builder;
    GtkWidget *log_win;
};


typedef struct user_button t_userbuton;
struct user_button{
    t_userbuton *next;
    GtkWidget * user_info[8];
    /* button conteiner image name_last time las_t grid_h grid_v*/
};
typedef struct user_massage t_user_message;
struct user_massage{
    t_user_message * next;
    GtkWidget * grid;
    GtkWidget * text_message;
    gboolean type_message; // TRUE - our text;
    GtkWidget * time;
};

typedef struct users_in_chat t_users_in_chat;
struct users_in_chat{
    t_users_in_chat * next;
    GtkWidget * image;
    GtkWidget * name;
};

typedef struct user_cha t_user_base;
struct user_cha{
    t_user_base * next;
    gint * type; // o is 1 user 1 man y user
    GtkWidget * image;
    t_user_message * messages; /*do struct  */// in message
    t_users_in_chat * user_contact;
    GtkWidget *bases;
    gint * leng_another_message;
    gint  leng_mes;
};
typedef struct our_user t_our_user;
typedef struct user_search  t_user_search;
struct user_search{
    t_user_search * next;
    GtkWidget * togle_but;
    GtkWidget * g_base;
    GtkWidget * label_name;
    GtkWidget * user_image;

};

typedef struct user_data t_user_data;
struct user_data{
    t_userbuton * user_but;
    t_user_base * user_ch;
    t_user_search * search;
    gint  * leng_user;
};


typedef struct uchat_bases t_uchat_bases;
     
struct uchat_bases{
    GtkWidget * stack_1;
    GtkWidget * chat_entry;
    GtkWidget * image_user;
    GtkWidget * grid_user_online;
    GtkWidget * chat_infos;
    GtkWidget * add_;
    GtkWidget * push;
    t_user_data * us_data;
    gint  grid_v;
    gint len_tags;
    GtkWidget * last_user_button;
    GtkWidget * active_grid;
    GtkAdjustment * adj;
    GtkWidget * user_names;
    GtkWidget * pop_searh;
    GtkWidget * grid_popv_teg;
    GtkWidget * grid_popov_users;
    GtkWidget * last_check;
    GtkWidget * chat_g_name;
    GtkWidget * iddes;
    GtkWidget * chat_namess;
};


void log_main_loop();
int  sign_check_correct(GtkWidget * er_label,GtkEntry * login, GtkEntry * password);
int  check_register(wig_log *widgets);
void uchat_base();
void  create_user_button(t_userbuton **t_user, int i);
char *mx_strnew(const int size, char c);
char *mx_itoa(int number);
void create_us_chat(t_uchat_bases * widgets);
void add_message(t_uchat_bases * widgest, int type, char * text_message, int chat);
void search_list(t_user_search  ** search, int i);
t_user_search *search_clear_list(t_user_search *head);
void user_toggled(GtkToggleButton * togglebutton, gpointer user_data );
void users_add(GtkToggleButton * button, t_uchat_bases * widgets);
