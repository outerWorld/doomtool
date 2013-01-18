
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>

#include "f1g_smart_conf.h"
#include "f1g_regset.h"

/*
 *
 */
static int read_rsp_settings(char *setting, regset_p p_rs)
{
	int i;
	int ret = -1;
	int item_num = 0;
	buffer_t rule;
	smart_conf_t sc;
	buffer_t rule_val;
	char item[ITEM_NAME_SIZE] = { 0 };

	buffer_init(&rule, 1024);
	buffer_init(&rule_val, 1024);

	if (F1G_OK != smart_conf_init(&sc, 10, 10)) {
		return -1;
	}

	if (F1G_OK != smart_conf_read(&sc, setting, CFT_INI)) {
		goto __load_exit;
	}

	if (F1G_OK != smart_conf_get_i32(&sc, "SPAWN", "item_num", 0, &item_num)) {
		goto __load_exit;
	}

	for (i=0; i<item_num; i++) {
		snprintf(item, ITEM_NAME_SIZE, "rule[%d]", i);
		if (F1G_OK != smart_conf_get_str(&sc, "SPAWN", item, "*", buffer_data(&rule), buffer_size(&rule))) {
			goto __load_exit;
		}
		snprintf(item, ITEM_NAME_SIZE, "value[%d]", i);
		if (F1G_OK != smart_conf_get_str(&sc, "SPAWN", item, "*", buffer_data(&rule_val), buffer_size(&rule_val))) {
			goto __load_exit;
		}

		if (F1G_OK != regset_addreg(p_rs, buffer_data(&rule), buffer_data(&rule_val), NULL)) goto __load_exit;
	}

	ret = 0;
__load_exit:
	buffer_destroy(&rule);
	buffer_destroy(&rule_val);
	smart_conf_destroy(&sc);

	return ret;
}

static void usage(char *exec_name)
{
}

#define LINE_SIZE	1024
static char cmd_line[LINE_SIZE] = { 0 };
static char line[LINE_SIZE] = { 0 };
/*
 * Usage: gospawn setting_file cmd cmd_parameters
 *   example: gospawn ssh.set ssh root@10.0.2.112
 */
int main(int argc, char *argv[])
{
	int i = 0;
	regset_t rs;
	FILE * pfile = NULL;
	char * setting_fpath;
	i8_p p_data;

	if (argc < 3) {
		usage(argv[0]);
		return 1;
	}
	
	setting_fpath = argv[1];

	for (i=2; i<argc; i++) {
		strcat(cmd_line, argv[i]);
		strcat(cmd_line, " ");
	}
	cmd_line[strlen(cmd_line)-1] = '\0';

	if (F1G_OK != regset_init(&rs, 10)) {
		return 1;
	}

	if (0 != read_rsp_settings(setting_fpath, &rs)) {
		return 1;
	}
	
	pfile = popen(cmd_line, "r");
	if (!pfile) return 1;

	while (fgets(line, LINE_SIZE, pfile)) {
		line[strlen(line)-1] = '\0';
		fprintf(stdout, "%s", line);
		fflush(stdout);
		if (F1G_OK == regset_match(&rs, line, &p_data)) {
			fprintf(stdout, "matched [%s] value[%s]\n", line, p_data);
			fflush(stdout);
			fwrite(p_data, strlen(p_data), 1, pfile);
			fwrite("\n", 1, 1, pfile);
		}
	}

	pclose(pfile);
	regset_destroy(&rs);

	return 0;
}
