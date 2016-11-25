#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/// parse Redis's info command response
/// \param data info response data
/// \param size info response size
/// \param callback a callback to get the data
/// \param lookup_key look for a specific key (to use with ret_value)
/// \param ret_value a pointer-to-a-pointer to set the lookup key value
void parse_all(const char *data, size_t *size, void (*callback)(const char *, const char *), const char * lookup_key, char **ret_value)
{
    char *token = NULL;
    char *info = malloc(*size);
    strncpy(info, data, *size);

    token = strtok(info, "\n");
    while( token != NULL )
    {
        char keyname[1024], value[1024];
        if (strlen(token) <=3 || (*token == '#')) {
            token = strtok(NULL, "\n");
            continue;
        }

        sscanf(token, "%32[^:]:%s", keyname, value);

        if (callback != NULL) {
            callback(keyname, value);
        } else {
            if (strcmp(keyname, lookup_key) == 0) {
                *ret_value = malloc(strlen(value));
                strcpy(*ret_value, value);
            }
        }


        token = strtok(NULL, "\n");
    }

    free(info);
}