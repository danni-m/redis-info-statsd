#ifndef REDIS_INFO_STATSD_PARSE_INFO_H
#define REDIS_INFO_STATSD_PARSE_INFO_H

void config_callback(const char *key, const char *val);
void parse_all(const char *data, size_t *size, void (*callback)(const char *, const char *), const char * lookup_key, char **ret_value);

#endif //REDIS_INFO_STATSD_PARSE_INFO_H
