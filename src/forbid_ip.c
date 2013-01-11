
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <errno.h>
#include <unistd.h>

#define LINE_SIZE	1024
static char cmdline[LINE_SIZE] = { 0 };
static char line[LINE_SIZE] = { 0 };
static char in_line[LINE_SIZE] = { 0 };

int main(int argc, char *argv[])
{
	int i = 0;
	FILE * cmdfp = NULL;
	char drop_ip[24] = { 0 };
	int find = 0;
	
#if 0
	for (i=1; i<argc; i++) {
		strcat(cmdline, argv[i]);	
		strcat(cmdline, " ");
	}
	cmdline[strlen(cmdline)-1] = '\0';
#endif
	while (fgets(drop_ip, 24, stdin)) {
		drop_ip[strlen(drop_ip)-1] = '\0';
		find = 0;
		snprintf(cmdline, LINE_SIZE-1, "sudo iptables %s %s", "-L", "-n");
		cmdfp = popen(cmdline, "r");
		if (!cmdfp) {
			fprintf(stderr, "popen %s failed [%d:%s]\n", cmdline, errno, strerror(errno));
			return 1;
		}

		while (fgets(line, LINE_SIZE, cmdfp)) {
			line[strlen(line)-1] = '\0';
			if (strstr(line, drop_ip)) {
				find = 1;
				break;
			}
			//fprintf(stdout, "output [%s]\n", line);
		}
		pclose(cmdfp);
		
		if (!find) {
			snprintf(cmdline, LINE_SIZE-1, "sudo iptables -I INPUT -s %s -j DROP", drop_ip);
			cmdfp = popen(cmdline, "r");
			fprintf(stdout, "%s:\n", cmdline);
			while (fgets(line, LINE_SIZE, cmdfp)) {
				line[strlen(line)-1] = '\0';
				fprintf(stdout, "\t%s\n", line);
			}
		
			pclose(cmdfp);
			fflush(stdout);
		} else {
			//fprintf(stdout, "[%s] found \n", drop_ip, cmdline);
		}
	}

	return 0;
}
