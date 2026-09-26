#ifndef HOMECONNECT_H
# define HOMECONNECT_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <ctype.h>
# include <stdint.h>
# include <math.h>
# include <pthread.h>
# include <sqlite3.h>
# include <time.h>
# include <modbus.h>
# include <signal.h>
# include <stdbool.h>
# include <limits.h>
# include <fcntl.h>
# include <errno.h>
# include <signal.h>
# include <microhttpd.h>
# include <libwebsockets.h>
# include <sys/ptrace.h>
# include <curl/curl.h>
# include <json-c/json.h>
# include <openssl/evp.h>
# include <sys/stat.h>
# include <openssl/rand.h>
# include <sys/reboot.h>

/* PATHS */

# define INSTALL_DIR	"/opt/homeconnect"
# define DATA_DIR	"/var/lib/homeconnect"
# define LOG_DIR	"/var/log/homeconnect"
# define CONFIG_DIR	INSTALL_DIR "/config"
# define BACKUP_DIR	INSTALL_DIR "/backups"
# define LOG_PATH	LOG_DIR "/homeconnect.log"
# define CONFIG_PATH	CONFIG_DIR "/homeconnect.conf"
# define DB_PATH	DATA_DIR "/homeconnect.db"

/* SQL */

/* MODBUS CONST */

/* LIMMITS */

/* TIMINGS (Seconds) */

/* SNIFING CONST */

/* STRUCTS */

/* FUNCTIONS */

/* MODBUS */

/* REGISTERS */

/* DATABASE */

/* CONTROL */

/* TOOLS */

/* API */

/* WEBSOCKET */

/* UPDATE */

#endif
