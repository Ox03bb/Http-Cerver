#ifndef CONFIG_H
#define CONFIG_H

typedef struct {
    char *host;
    int port;
    char *public_dir;
    int enable_ssl;
    char *cert_file;
    char *key_file;
} ServerConfig;

typedef struct {
    int enable;
    char *prefix;
    char *target_ip;
    int target_port;
} ProxyConfig;

typedef struct {
    char *log_file;
} LoggingConfig;

typedef struct {
    int auto_index;
} FeaturesConfig;

typedef struct {
    ServerConfig server;
    ProxyConfig proxy;
    LoggingConfig logging;
    FeaturesConfig features;
} Config;

int load_config(const char *filename, Config *cfg);

#endif // CONFIG_H