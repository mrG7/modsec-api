
//#undef inline
#define	inline inline

#include <cstdio>
#include "api.h"
#include "ModSecJNIwrapper.h"

char *config_file = NULL;
char *event_files[1024];
int event_file_cnt;
char *event_file = NULL;
int event_file_len = 0;
char **event_file_lines;
int event_line_cnt = 0;
int event_file_blocks[256];

unsigned int bodypos = 0;
unsigned int responsepos = 0;

directory_config *dir_config;

#define	EVENT_FILE_MAX_SIZE		(16*1024*1024)

apr_status_t readbody(request_rec *r, char *buf, unsigned int length, unsigned int *readcnt, int *is_eos);
apr_status_t readresponse(request_rec *r, char *buf, unsigned int length, unsigned int *readcnt, int *is_eos);

void alloc_str(char **str, unsigned int size)
{
    *str = (char *) malloc( sizeof(char) * size );
}

void copy_str(char ** dest , const char *src)
{
    alloc_str( dest , strlen(src));
	strcpy( *dest , src );
}

void readeventfile(char *req , unsigned int req_sz)
{
	if(event_file == NULL)
	{
		event_file = req;
		event_file_lines = (char **)malloc(EVENT_FILE_MAX_SIZE);
	}

	event_line_cnt = 0;
	memset(event_file_blocks, -1, sizeof(int) * 256);

    event_file = req;
    event_file_len = req_sz;

	event_file[event_file_len] = 0;
}

void garbageCollect()
{
    free(event_file);
    free(event_file_lines);
    event_file = NULL;
    event_file_len = 0;
    event_file_lines = NULL;
    event_line_cnt = 0;
    bodypos = 0;
    responsepos = 0;
}

void parseeventfile()
{
	if(event_file_len == 0 || event_file == NULL)
		return;

	char *t = event_file;
	char *e = event_file + event_file_len;

	while(t < e)
	{
		event_file_lines[event_line_cnt++] = t;

		while(t < e && *t != 10 && *t != 13)
			t++;

		while(t < e && (*t == 10 || *t == 13))
			*t++ = 0;
	}

	for(int i = 0; i < event_line_cnt; i++)
	{
		int l = strlen(event_file_lines[i]);

		if(l != 14)
			continue;

		if( event_file_lines[i][0] != '-' || event_file_lines[i][1] != '-' || 
            event_file_lines[i][l-2] != '-' || event_file_lines[i][l-1] != '-')
			continue;

		char blk =  event_file_lines[i][l-3];

		event_file_blocks[blk] = i;
	}
}

void log(void *obj, int level, char *str)
{
	printf("%s\n", str);
}

apr_status_t readbody(request_rec *r, char *buf, unsigned int length, unsigned int *readcnt, int *is_eos)
{
	int j = event_file_blocks['C'];
    printf("j:%d\n",j);

	if(j < 0)
	{
		*is_eos = 1;
		return APR_SUCCESS;
	}

	j++;

	if(event_file_lines[j][0] == 0)
	{
		*is_eos = 1;
		return APR_SUCCESS;
	}

	unsigned int l = strlen(event_file_lines[j]);
	unsigned int size = length;

	if(bodypos + size > l)
		size = l - bodypos;

	memcpy(buf, &event_file_lines[j][bodypos], size);

	bodypos += size;
	*readcnt = size;

	if(bodypos == l)
		*is_eos = 1;

	return APR_SUCCESS;
}

apr_status_t readresponse(request_rec *r, char *buf, unsigned int length, unsigned int *readcnt, int *is_eos)
{
	int j = event_file_blocks['G'];

	if(j < 0)
	{
		*is_eos = 1;
		return APR_SUCCESS;
	}

	j++;

	if(event_file_lines[j][0] == 0)
	{
		*is_eos = 1;
		return APR_SUCCESS;
	}

	unsigned int l = strlen(event_file_lines[j]);
	unsigned int size = length;

	if(responsepos + size > l)
		size = l - responsepos;

	memcpy(buf, &event_file_lines[j][responsepos], size);

	responsepos += size;
	*readcnt = size;

	if(responsepos == l)
		*is_eos = 1;

	return APR_SUCCESS;
}

int initModSecEngine(char *config_file)
{    
    int status = 0;
    
    if(config_file == NULL)
	{
		printf("No config file provided\n");
		return 0;
	}

	modsecSetLogHook(NULL, log);

	modsecSetReadBody(readbody);
	//modsecSetReadResponse(readresponse);

	modsecInit();

	modsecStartConfig();

	dir_config = modsecGetDefaultConfig();
    	
    const char * err = modsecProcessConfig(dir_config, config_file);

	if(err != NULL)
	{
		printf("%s\n", err);
        return 0;
	}

	modsecFinalizeConfig();

	modsecInitProcess();

    return 1;
}

void terminateModSecEngine()
{
	modsecTerminate();
    return;
}

int processRequest(char *raw_req)
{
    printf("ModSecurity is processing request(C)\n");
    int status = 0;
    copy_str(&event_files[0], raw_req);
    event_file_cnt = 1;
    
    conn_rec *c = NULL;
    request_rec *r = NULL;

    for(int i = 0; i < event_file_cnt; i++)
	{
		readeventfile(event_files[i], strlen(event_files[i]));
		parseeventfile();

		c = modsecNewConnection();

		modsecProcessConnection(c);
        
		r = modsecNewRequest(c, dir_config);

		int j = event_file_blocks['B'];
        
		if(j < 0)
			continue;

		j++;

		if(event_file_lines[j][0] == 0)
			continue;
		
        char *method = event_file_lines[j];
		char *url = strchr(method, 32);
		char *proto = strchr(url + 1, 32);
		if(url == NULL || proto == NULL)
			continue;

		*url++=0;
		*proto++=0;
#define	SETMETHOD(m) if(strcmp(method,#m) == 0){ r->method = method; r->method_number = M_##m; }

		r->method = "INVALID";
		r->method_number = M_INVALID;

		SETMETHOD(OPTIONS)
		SETMETHOD(GET)
		SETMETHOD(POST)
		SETMETHOD(PUT)
		SETMETHOD(DELETE)
		SETMETHOD(TRACE)
		SETMETHOD(CONNECT)
		SETMETHOD(MOVE)
		SETMETHOD(COPY)
		SETMETHOD(PROPFIND)
		SETMETHOD(PROPPATCH)
		SETMETHOD(MKCOL)
		SETMETHOD(LOCK)
		SETMETHOD(UNLOCK)

		r->protocol = proto;

		while(event_file_lines[++j][0] != 0)
		{
			char *value = strchr(event_file_lines[j], ':');

			if(value == NULL)
				break;

			*value++ = 0;

			while(*value <=32 && *value != 0)
				value++;

			apr_table_setn(r->headers_in, event_file_lines[j], value);
		}

		r->content_encoding = apr_table_get(r->headers_in, "Content-Encoding");
		r->content_type = apr_table_get(r->headers_in, "Content-Type");
		r->hostname = apr_table_get(r->headers_in, "Host");
		r->path_info = url;
		
		char *query = strchr(url, '?');
		char *rawurl = url;

		if(query != NULL)
		{
			rawurl = (char *)apr_palloc(r->pool, strlen(url) + 1);
			strcpy(rawurl, url);
			*query++ = 0;
			r->args = query;
		}

		const char *lng = apr_table_get(r->headers_in, "Content-Languages");

		if(lng != NULL)
		{
			r->content_languages = apr_array_make(r->pool, 1, sizeof(const char *));

			*(const char **)apr_array_push(r->content_languages) = lng;
		}

		r->request_time = apr_time_now();

		copy_str(&(r->parsed_uri.scheme), "http");
		r->parsed_uri.path = r->path_info;
		r->parsed_uri.hostname = (char *)r->hostname;
		r->parsed_uri.is_initialized = 1;
		r->parsed_uri.port = 80;
		copy_str(&(r->parsed_uri.port_str), "80");
		r->parsed_uri.query = r->args;
		r->parsed_uri.dns_looked_up = 0;
		r->parsed_uri.dns_resolved = 0;
		r->parsed_uri.password = NULL;
		r->parsed_uri.user = NULL;
		r->parsed_uri.fragment = NULL;

		r->unparsed_uri = rawurl;
		r->uri = r->unparsed_uri;

		r->the_request = (char *)apr_palloc(r->pool, strlen(r->method) + 1 + strlen(r->uri) + 1 + strlen(r->protocol) + 1);

		strcpy(r->the_request, r->method);
		strcat(r->the_request, " ");
		strcat(r->the_request, r->uri);
		strcat(r->the_request, " ");
		strcat(r->the_request, r->protocol);

		apr_table_setn(r->subprocess_env, "UNIQUE_ID", "1");
        
		status = modsecProcessRequest(r);
        modsecProcessResponse(r);
        modsecFinishRequest(r);
	}
    return status;
}

JNIEXPORT jint JNICALL Java_vulnapp_modsecurity_wrappers_ModSecurityWrapper_wrapInitModSecEngine
  (JNIEnv *jenv, jobject obj, jstring config)
{
    
    printf("Starting mod security Engine(C)I\n");
    const char *conf;
    char *conf_copy;
    
    conf = (jenv)->GetStringUTFChars(config, NULL);
    copy_str(&conf_copy, conf);

    initModSecEngine(conf_copy);
    
    free(conf_copy);
    (jenv)->ReleaseStringUTFChars(config, conf);
    return 0;
}

JNIEXPORT jint JNICALL Java_vulnapp_modsecurity_wrappers_ModSecurityWrapper_wrapTerminateModSecEngine
  (JNIEnv *env, jobject obj)
{
    terminateModSecEngine();
    return 0;
}

JNIEXPORT jint JNICALL Java_vulnapp_modsecurity_wrappers_ModSecurityWrapper_wrapFilterRawRequest
  (JNIEnv *env, jobject obj, jstring config, jstring rawRequest)
{
    jint status = 0;
    printf("Modsecurity plugin initiated\n");

    const char *conf, *req;
    char *conf_copy, *req_copy;
   
    conf = NULL; 
    req  = NULL; 
    
    conf = (env)->GetStringUTFChars(config, NULL);
    req  = (env)->GetStringUTFChars(rawRequest, NULL);
    
    copy_str(&conf_copy, conf);
    copy_str(&req_copy, req);

    (env)->ReleaseStringUTFChars(config, conf);
    (env)->ReleaseStringUTFChars(rawRequest, req);
    
    if(req_copy == NULL){
        printf("No request provided\b");
        return 0;
    }
 
    status = processRequest( req_copy );

    garbageCollect();
    
    free(conf_copy);
    free(req_copy);

    return status;
}

/*
void JNI_OnUnload(JavaVM *vm, void *reserved)
{
    printf("Unloading JNI...\n");
}
*/
/*
jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    return JNI_VERSION_1_6; 
}
*/

int main(int argc, char *argv[])
{
    char *config , *raw_req, *event_file_path;

    copy_str(&config, argv[1]);
    copy_str(&event_file_path , argv[2]);
    
    FILE *fp = fopen(event_file_path, "rb");
        
	raw_req = (char *) malloc(EVENT_FILE_MAX_SIZE);
    int file_len = fread(raw_req, 1, EVENT_FILE_MAX_SIZE - 1, fp);
    raw_req[file_len] = 0;
    initModSecEngine(config);    
    
    int status = processRequest(raw_req);
    printf("status:%d\n",status);
	
    terminateModSecEngine();
	return 0;
}
