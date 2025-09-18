#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <toml.h>

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

int load_config(const char *filename, Config *cfg) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("fopen");
        return -1;
    }

    char errbuf[200];
    toml_table_t *conf = toml_parse_file(fp, errbuf, sizeof(errbuf));
    fclose(fp);
    if (!conf) {
        fprintf(stderr, "TOML parse error: %s\n", errbuf);
        return -1;
    }

    // -------------------
    // [server]
    // -------------------
    toml_table_t *server = toml_table_in(conf, "server");
    if (server) {
        toml_datum_t host = toml_string_in(server, "host");
        if (host.ok)
            cfg->server.host = strdup(host.u.s);

        toml_datum_t port = toml_int_in(server, "port");
        if (port.ok)
            cfg->server.port = (int)port.u.i;

        toml_datum_t pub = toml_string_in(server, "public_dir");
        if (pub.ok)
            cfg->server.public_dir = strdup(pub.u.s);

        toml_datum_t ssl = toml_bool_in(server, "enable_ssl");
        if (ssl.ok)
            cfg->server.enable_ssl = ssl.u.b;

        toml_datum_t cert = toml_string_in(server, "cert_file");
        if (cert.ok)
            cfg->server.cert_file = strdup(cert.u.s);

        toml_datum_t key = toml_string_in(server, "key_file");
        if (key.ok)
            cfg->server.key_file = strdup(key.u.s);
    }

    // -------------------
    // [proxy]
    // -------------------
    toml_table_t *proxy = toml_table_in(conf, "proxy");
    if (proxy) {
        toml_datum_t en = toml_int_in(proxy, "enable");
        if (en.ok)
            cfg->proxy.enable = (int)en.u.i;

        toml_datum_t pre = toml_string_in(proxy, "prefix");
        if (pre.ok)
            cfg->proxy.prefix = strdup(pre.u.s);

        toml_datum_t ip = toml_string_in(proxy, "target_ip");
        if (ip.ok)
            cfg->proxy.target_ip = strdup(ip.u.s);

        toml_datum_t tp = toml_int_in(proxy, "target_port");
        if (tp.ok)
            cfg->proxy.target_port = (int)tp.u.i;
    }

    // -------------------
    // [logging]
    // -------------------
    toml_table_t *logging = toml_table_in(conf, "logging");
    if (logging) {
        toml_datum_t log = toml_string_in(logging, "log_file");
        if (log.ok)
            cfg->logging.log_file = strdup(log.u.s);
    }

    // -------------------
    // [features]
    // -------------------
    toml_table_t *features = toml_table_in(conf, "features");
    if (features) {
        toml_datum_t auto_idx = toml_bool_in(features, "auto_index");
        if (auto_idx.ok)
            cfg->features.auto_index = auto_idx.u.b;
    }

    toml_free(conf);
    return 0;
}
