#pragma once

#include <limits.h>

#include "http_core.h"
#include "http_request.h"

#include "modsecurity.h"
#include "apache2.h"
#include "http_main.h"
#include "http_connection.h"

#include "apr_optional.h"
#include "mod_log_config.h"

#include "msc_logging.h"
#include "msc_util.h"

#include "ap_mpm.h"
#include "scoreboard.h"

#include "apr_version.h"

#include "apr_lib.h"
#include "ap_config.h"
#include "http_config.h"


#ifdef	__cplusplus
extern "C"
{
#endif

server_rec *modsecInit();
void modsecTerminate();

void modsecStartConfig();
directory_config *modsecGetDefaultConfig();
const char *modsecProcessConfig(directory_config *config, const char *dir);
void modsecFinalizeConfig();

void modsecInitProcess();

conn_rec *modsecNewConnection();
void modsecProcessConnection(conn_rec *c);

request_rec *modsecNewRequest(conn_rec *connection, directory_config *config);
int modsecProcessRequest(request_rec *r);
int modsecProcessResponse(request_rec *r);
int modsecFinishRequest(request_rec *r);

void modsecSetLogHook(void *obj, void (*hook)(void *obj, int level, char *str));

void modsecSetReadBody(apr_status_t (*func)(request_rec *r, char *buf, unsigned int length, unsigned int *readcnt, int *is_eos));
void modsecSetReadResponse(apr_status_t (*func)(request_rec *r, char *buf, unsigned int length, unsigned int *readcnt, int *is_eos));
void modsecSetWriteBody(apr_status_t (*func)(request_rec *r, char *buf, unsigned int length));
void modsecSetWriteResponse(apr_status_t (*func)(request_rec *r, char *buf, unsigned int length));

#ifdef __cplusplus
}
#endif
