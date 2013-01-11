/*
 * GoU --> go you, in chinese, it means 走你
 *   it will be a http server.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <time.h>
#include <unistd.h>
#include <signal.h>

#include "f1g_basic_funcs.h"
#include "f1g_server.h"

static i8_p http_header_rsp = //"Cache-Control:private, max-age=0, must-revalidate\r\n"
							  //"Connection:keep-alive\r\n"
							  //"Connection:Close\r\n" 
							  "Content-Type:text/html; charset=utf-8\r\n"	\
							  //"Date:Fri, 11 Jan 2013 05:50:50 GMT\r\n"
							  "Server:GoU 0.0.1 \r\n";
static i8_p http_content = "<html><head><title>Test Server GoU</title></head><body><h1>GoU, the best choice.</h1></body></html>";

/*
 *
 */
static i32_t http_proc(context_t ctx, buffer_p p_rdbuf, buffer_p p_wrbuf, i32_t *proc_st)
{

	// Http resolver

	// url resolver

	// response
	buffer_append_format(p_wrbuf, "%s", "HTTP/1.1 200 OK\r\n");
	buffer_append_format(p_wrbuf, "%s", http_header_rsp);
	buffer_append_format(p_wrbuf, "Conent-Length: %d\r\n", strlen(http_content));
	buffer_append_format(p_wrbuf, "\r\n%s", http_content);
	//buffer_show(p_wrbuf);

	*proc_st = PROC_ST_SENDBUF;

	return F1G_OK;
}


/*
 * Usage: GoU --serv_conf=conf_file -d 
 *	'--serv_conf=conf_file' or '-c conf_file': specify the file path of server framework.
 *  '-d': running under deamon mode, if not specified, it runs on console mode.
 */
int main(int argc, char *argv[])
{
	i32_t 	result;
	i32_t	is_daemon = 0;
	i8_p 	fpath = NULL;
	server_conf_t conf;
	serv_object_p p_serv = NULL;

	while (-1 != (result = getopt(argc, argv, "c:d"))) {
		switch (result) {
			case 'c':
				if (!optarg || strlen(optarg) < 0) {
					fpath = DEFAULT_SERV_CONF_FPATH;
				} else {
					fpath = optarg;
				}
				fprintf(stdout, "opt=%c, optopt=%c, optarg=%s\n", *(i8_p)&result, optopt, optarg);
				break;
			case 'd':
				is_daemon = 1;
				fprintf(stdout, "opt=%c, optopt=%c, optarg=%s\n", *(i8_p)&result, optopt, optarg);
				break;
			default:
				break;
		}
	}

	if (is_daemon) {
		if (F1G_OK != set_daemon(argv[0])) {
			fprintf(stderr, "%s set deamon fail \n", argv[0]);
			return 1;
		}
	}
	signal(SIGPIPE, SIG_IGN);
	
	if (F1G_OK != serv_load(fpath, &conf)) {
		return 1;
	}

	if (!(p_serv = serv_create(&conf))) {
		return 1;
	}

	if (F1G_OK != serv_set_work_cb(p_serv, NULL, http_proc, NULL, 0, NULL)) {
		return 1;
	}

	if (F1G_OK != serv_run(p_serv)) {
		return 1;
	}

	while (1) {
		sleep(5);
	}

	serv_destroy(p_serv);
	free(p_serv);

	return 0;
}
