
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LINE_SIZE	2048
static char line[LINE_SIZE] = { 0 };

#define IP_STR_SIZE 24
typedef struct _cracker_s {
	char 	hack_ip[IP_STR_SIZE];
	int		last_hack_time;
	int		hack_times;
	int 	hack_freq;
}cracker_t, *cracker_p;

/*
 * usage: ssh_hack_defend
 */
int main(int argc, char *argv[])
{
	while (fgets(line, LINE_SIZE, stdin)) {
		line[strlen(line)-1] = '\0';
	}

	return 0;
}
