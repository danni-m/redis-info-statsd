#include <string.h>
#include <pthread.h>
#include <rmutil/util.h>
#include <redismodule.h>
#include <rmutil/logging.h>
#include <hiredis/hiredis.h>
#include <stdlib.h>
#include "parse_info.h"
#include "statsd-c-client/statsd-client.h"

pthread_t info_thread;
redisContext *redis_connection;
statsd_link *statsd_connection;
struct timespec update_interval;

void send_to_statsd(const char *key, const char * value)
{
    long ret = strtol(value, NULL, 10);
    if (ret == 0 && strcmp(value, "0") != 0) {
        return;
    }

    statsd_gauge(statsd_connection, key, ret);
}

void *info_loop(void *d){

    redisReply *reply;

    while (1) {
        nanosleep(&update_interval, NULL);
        reply = redisCommand(redis_connection, "INFO ALL");
        parse_all(reply->str, &reply->len, &send_to_statsd, NULL, NULL);
        freeReplyObject(reply);
    }
    return NULL;
}


void connect_to_server(int port)
{
    struct timeval timeout = { 1, 500000 }; // 1.5 seconds
    redis_connection = redisConnectWithTimeout("localhost", port, timeout);
    if (redis_connection == NULL || redis_connection->err) {
        if (redis_connection) {
            printf("Connection error: %s\n", redis_connection->errstr);
            redisFree(redis_connection);
        } else {
            printf("Connection error: can't allocate redis context\n");
        }
    }
}

int RedisModule_OnLoad(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    char  *statsd_server = "127.0.0.1";
    char *statsd_namespace = "redis";
    long long statsd_server_port = 8125;

    if (RedisModule_Init(ctx,"redis-statsd",1,REDISMODULE_APIVER_1)
        == REDISMODULE_ERR) return REDISMODULE_ERR;

    if (argc == 2) {
        RMUtil_ParseArgs(argv, argc, 0, "cl", &statsd_server, &statsd_server_port);
    } else if (argc == 3) {
        RMUtil_ParseArgs(argv, argc, 0, "clc", &statsd_server, &statsd_server_port, &statsd_namespace);
    }
    else if (argc != 0 ) {
        RM_LOG_WARNING(ctx, "statsd configuration: server=%s port=%d", statsd_server, statsd_server_port);
        return REDISMODULE_ERR;
    }

    statsd_connection = statsd_init_with_namespace(statsd_server, (int) statsd_server_port, statsd_namespace);

    // create an hiredis client, connect the server by parsing the configuration
    RedisModuleCallReply *reply;
    reply = RedisModule_Call(ctx,"INFO","c","server");
    size_t len;
    const char *info = RedisModule_CallReplyStringPtr(reply, &len);
    char *port;
    parse_all(info, &len, NULL, "tcp_port", &port);
    RedisModule_FreeCallReply(reply);

    connect_to_server(atoi(port));

    // set the update time at every 1 second
    update_interval.tv_sec = 1;
    update_interval.tv_nsec = 0;

    if (pthread_create(&info_thread, NULL, info_loop, NULL)) {
        RM_LOG_WARNING(ctx, "Error creating thread");
        return REDISMODULE_ERR;
    }

    return REDISMODULE_OK;
}