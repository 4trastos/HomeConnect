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

# define INSTALL_DIR	        "/opt/homeconnect"
# define DATA_DIR	            "/var/lib/homeconnect"
# define LOG_DIR	            "/var/log/homeconnect"
# define CONFIG_DIR	INSTALL_DIR "/config"
# define BACKUP_DIR	INSTALL_DIR "/backups"
# define LOG_PATH LOG_DIR       "/homeconnect.log"
# define CONFIG_PATH CONFIG_DIR "/homeconnect.conf"
# define DB_PATH DATA_DIR       "/homeconnect.db"

/* SQL */

# define SQL_SAVE_CONFIG        "INSERT OR IGNORE INTO config (key, value) VALUES (?, ?)"
# define SQL_SAVE_SCHEDULER     "INSERT OR IGNORE INTO scheduler (id, name, active, action, hour,mon, tue, wed, thu, fri, sat, sun,mode, target_temp) VALUE (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"

/* MODBUS CONST */

/* LIMMITS */

# define MAX_WS_CLIENTS 5

/* TIMINGS (Seconds) */

/* SNIFING CONST */

extern const char *SCHEMA_SQL;

/* STRUCTS */

typedef struct s_modbus_cmd
{

}   t_modbus_cmd;

typedef struct s_modbus_queue
{

}   t_modbus_queue;

typedef struct  s_thread_ctrl
{
    pthread_t           tid;
    pthread_mutex_t     mutex;
    t_thread_status     status;
    int                 unit_id;                    // qué cooler controla este hilo
    int                 padding[2]; 
}   t_thread_ctrl;

typedef struct s_scheduler
{
    int                 id;
    char                name[64];
    int                 active;
    char                action[8];
    char                hour[8];
    int                 mon;
    int                 tue;
    int                 wed;
    int                 thu;
    int                 fri;
    int                 sat;
    int                 sun;
    char                mode[64];
    float               target_temp;
}   t_scheduler;

typedef struct  s_cameras
{
    int                 id;
    int                 on;
}   t_cameras;

typedef struct s_irrigation
{
    int                 id;
    int                 on;
}   t_irrigation;

typedef struct s_alarm
{
    int                 id;
}   t_alarm;

typedef struct s_sensor
{
    int                 id;
    char                type[4];             // "INT" - "EXT"
    float               temperature;
    float               humidity;
}   t_sensor;

typedef struct s_heating
{
    int                 id;
    int                 on;                 // 0=off,   1=on
    float               target_temp;
}   t_heating;

typedef struct s_cooler
{
    int                 id;                 // dirección ModbUs
    int                 on;                 // 0=off,    1=on
    int                 cool;               // 0=fan,    1=cool
    int                 fan_speed;          // 0=low,    1=high
    char                model[9];           // "Breezair"
    float               target_temp;
}   t_cooler;

typedef struct s_homeconnect
{
    modbus_t            *modbus_ctx;
    sqlite3             *db;
    t_cooler            *cooler;
    t_heating           *heating;
    t_sensor            *sensors;
    t_alarm             *alarm;
    t_irrigation        *irrigation;
    t_scheduler         *scheduler;
    t_modbus_queue      *modbus_queue;
    int                 num_sensor;
    int                 num_cameras;
    int                 num_schedulers;
    struct MHD_Daemon   *api_daemon;
    struct lws_context  *ws_context;
    struct lws          *ws_clients[MAX_WS_CLIENTS];
    t_thread_ctrl       api_thread;
    t_thread_ctrl       modbus_thread;
    int                 setup_mode;
    int                 read_interval;
    int                 ws_num_clients;
    char                active_token[64];   // token activo
    time_t              token_expiry;       // timestmap expiración
}   t_hommeconnect;

/* FUNCTIONS */

int main(int argc, char **argv);

/* MODBUS */

/* REGISTERS */

/* DATABASE */

/* CONTROL */

int ft_debugger(void);

/* TOOLS */

/* API */

/* WEBSOCKET */

/* UPDATE */

#endif
