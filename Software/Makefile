NAME		= homeconnect
CC		= gcc
CFLAGS		= -Wall -Werror -Wextra -Wpadded
DFLAGS		= $(CFLAGS) -fsanitize=address -g3
SFLAGS		= $(CFLAGS) -s
INCLUDES	= -Iincl -I/usr/include/modbus
LDLIBS		= -lmodbus -lsqlite3 -lm -lpthread -lmicrohttp -lwebsockets -lssl -lcrypto -lcurl -ljson-c

RM		= rm -f
DB_DIR		= /var/lib/homeconnect

COMMON_SRC	=

SRC		= src/main.c \
		  $(COMMON_SRC)

OBJS		= $(SRC:.c=.o)

all: setup $(NAME)

$(NAME): $(OBJS)
