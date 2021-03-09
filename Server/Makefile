NAME = libmx.a

LIBMX = libmx

UCHAT_SERVER = uchat_server

SERVER = server

LOG = log

DATABASE = database

JSON = json

SRC = src

OBJ = obj

INC = inc

FLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic

all: $(UCHAT_SERVER) clean

$(UCHAT_SERVER) :
	@make -C ./libmx 
	@clang main.c 	$(LOG)/$(SRC)/*.c \
					$(SERVER)/$(SRC)/*.c \
					$(DATABASE)/$(SRC)/*.c \
					$(JSON)/$(SRC)/*.c \
					-l sqlite3 -L $(LIBMX) -lmx -o $@

uninstall: clean
	@rm -rf libmx/$(NAME)
	@rm -rf $(UCHAT_SERVER)

clean:
	@rm -rf $(OBJ)

reinstall: uninstall all
