NAME = homeconnect

# Compilación Nativa (Rasberry Pi actual)
CC = gcc
# Cross-Compilation ARM 32-bits (Descomentar para producción)
#CC = arm-linux-gnueabihf-gcc
# Cross-Compilation ARM 64-bits (Descomentar para producción)
#CC = aarch64-linux-gnu-gcc

DB_DIR = /var/lib/homeconnect

LDLIBS = -lmodbus -lsqlite3 -lm -lpthread -lmicrohttpd -lwebsockets -lssl -lcrypto -lcurl -ljson-c

# Flags del preprocesador (Includes y Defines)
CPPFLAGS = -Iincl -I/usr/include/modbus -DDATA_DIR=\"$(DB_DIR)\"

# Flags del compilador (Opciones y Advertencias)
CFLAGS = -Wall -Werror -Wextra -Wpadded
DFLAGS = -Wall -Werror -Wextra -g3 -fsanitize=address -O0 -DDEBUG_ON
SFLAGS = -s

RM = rm -f

COMMON_SRC	= control/debug.c
SRC	= src/main.c $(COMMON_SRC)
OBJS	= $(SRC:.c=.o)

all: setup $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(SFLAGS) $(OBJS) -o $(NAME) $(LDLIBS)


debug: CFLAGS = $(DFLAGS)
debug: fclean $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

setup:
	@mkdir -p $(DB_DIR)
	@sudo chown $$USER:$$USER $(DB_DIR)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re setup debug
