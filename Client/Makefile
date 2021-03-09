CC = clang
LIB = libmx.a
NAME = uchat

base = main 

login = log_main_loop sign_check_correct check_register uchat_base create_user_button \
	mx_itoa mx_strnew create_us_chat add_message search_list search_clear_list users_add

net_functions = Connect Inet_pton Socket 

json_parsing = dup_callbacks dup_user_info set_user_info_from_json \
				dup_chat_info set_chat_info_from_json \
				dup_message_info set_message_info_from_json \
				dup_respons set_respons_from_json dup_client_info \
				dup_tag_info set_tag_info_from_json 

send_requests  = add_member_by_id  create_chat delete_chat \
				delete_message edit_message get_chat_info \
				login_user registr_user send_message \
				send_request search_by_tag_id get_all_tags

respons_handlle_funcs = receive_respons respons_handler 

dictionary = addItem delItem dictAllloc dictDealloc getItem 

dirs = base net_functions json_parsing send_requests dictionary respons_handlle_funcs login

FILES = $(foreach dir, $(dirs), $($(dir):%=$(dir)/%))

SRC_DIR = src/
OBJ_DIR = obj/
LIB_DIR = libmx/

SRC = $(FILES:%=$(SRC_DIR)%.c)
OBJ = $(FILES:%=$(OBJ_DIR)%.o)
LIB_PATH = $(LIB_DIR)$(LIB)

WFLAGS = -Wall -Wextra  -Wpedantic -Wno-unused-command-line-argument \
	-Wno-unused-variable -Wno-unused-function -Wno-unused-parameter

LFLAGS = -Iinc -I$(LIB_DIR)inc -I/usr/local/opt/openssl/include \
	-L/usr/local/opt/openssl/lib -lssl -lcrypto -L.

CFLAGS = -Ofast -march=native -flto -rdynamic

PKGCONFIG = `pkg-config --libs --cflags gtk+-3.0`
PGKCONFIGFLAGS = `pkg-config --cflags gtk+-3.0`

COMPILE = $(CC) -std=c11 -pipe $(WFLAGS) $(LFLAGS)
EXEC_IT = make -sf Makefile
EXEC_LD = $(EXEC_IT) -C $(LIB_DIR)
PRINT = printf
MKDIR = mkdir -p
RM = /bin/rm -rf
TARGET = release

R = \033[1;91m
G = \033[1;92m
Y = \033[1;93m
B = \033[1;94m
M = \033[1;95m
C = \033[1;96m
S = \033[38;5;45;1m
D = \033[0m
F = \033[5m
A = \033[A
K = \033[K

all: $(LIB) $(NAME)

$(OBJ_DIR):
	@$(MKDIR) $@ $(foreach dir, $(dirs), $@/$(dir))

$(LIB):
	@$(EXEC_LD)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@$(PRINT) "$K$G COMPILING  $Y[$M$(TARGET)$Y] $B$(<:$(SRC_DIR)%=%)$D\r"
	@$(COMPILE) $(CFLAGS) $(PGKCONFIGFLAGS) -o $@ -c $<

$(NAME): $(OBJ_DIR) $(OBJ)
	@$(PRINT) "$K$G COMPILING $Y[$M$(TARGET)$Y] $R$(NAME)$D\r"
	@$(COMPILE) $(CFLAGS) $(PKGCONFIG) $(LIB_PATH) $(OBJ) -o $(NAME)
	@$(PRINT) "$K"

clean:
	@$(EXEC_LD) $@
	@$(RM) $(OBJ_DIR)

uninstall:
	@$(EXEC_LD) $@
	@$(RM) $(OBJ_DIR) $(NAME)

reinstall: uninstall all

.PHONY: all clean uninstall reinstall
