
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define IP_STR_SIZE	24
#define USER_SIZE	128
#define LINE_SIZE	2048
static char line[LINE_SIZE] = { 0 };

#define IP_STR_SIZE 24
typedef struct _cracker_s {
	char 	hack_ip[IP_STR_SIZE];
	int		last_hack_time;
	int		hack_times;
	int 	hack_total;
}cracker_t, *cracker_p;

typedef struct _cracker_list_s {
	int			num;
	int			size;
	cracker_p 	crackers;
}cracker_list_t, *cracker_list_p;
static cracker_list_t crk_list = { 0  };

static int extend_size(cracker_list_p p_obj, int size)
{
	cracker_p p = 0;
	
	p = p_obj->crackers;
	p_obj->crackers = (cracker_p)malloc(size*sizeof(cracker_t));
	if (!p_obj->crackers) { return -1; }
	memset(p_obj->crackers, 0x00, size*sizeof(cracker_t));

	if (p_obj->num > 0) {
		memcpy(p_obj->crackers, p, p_obj->num*sizeof(cracker_t));
	}
	p_obj->size = size;

	if (p) { free(p); }
	
	return 0;
}


/*
 * usage: ssh_hack_defend
 */
int main(int argc, char *argv[])
{
	int i = 0;
	long time = 0;
	long interval = 0;
	char ip[IP_STR_SIZE] = { 0 };	
	char user[USER_SIZE] = { 0 };

	while (fgets(line, LINE_SIZE, stdin)) {
		line[strlen(line)-1] = '\0';
		sscanf(line, "%ld %s %s", &time, user, ip);
		//fprintf(stdout, "time=%ld, user=%s, ip=%s\n", time, user, ip);
		for (i=0; i<crk_list.num; i++) {
			if (strcmp(ip, crk_list.crackers[i].hack_ip) == 0) {
				break;
			}
		}
		if (i >= crk_list.num) {
			if (crk_list.num >= crk_list.size) {
				extend_size(&crk_list, 2*(crk_list.size+1));
			}
			
			// add one hacker
			strcpy(crk_list.crackers[i].hack_ip, ip);
			crk_list.crackers[i].last_hack_time = time;
			crk_list.crackers[i].hack_times = 1;
			crk_list.crackers[i].hack_total = 0;
			crk_list.num++;
		} else {
			interval = 	time - crk_list.crackers[i].last_hack_time;
			if (interval >= 900) { // interval with 900s is thought as normal.
				crk_list.crackers[i].hack_total = 0;
				crk_list.crackers[i].hack_times = 0;
			} else {
				crk_list.crackers[i].hack_total += interval;
			}
			crk_list.crackers[i].last_hack_time = time;
			crk_list.crackers[i].hack_times++;
		}

		// average 30s is thought as hacking.
		if (crk_list.crackers[i].hack_times>=10 && crk_list.crackers[i].hack_total*1.0/crk_list.crackers[i].hack_times <= 30.0) {
			//fprintf(stdout, "%s %d\n", crk_list.crackers[i].hack_ip, crk_list.crackers[i].hack_times);
			fprintf(stdout, "%s\n", crk_list.crackers[i].hack_ip);
			fflush(stdout);
			if (i < crk_list.num-1) {
				// cover this position by the last one.
				memcpy(&crk_list.crackers[i], &crk_list.crackers[crk_list.num-1], sizeof(cracker_t));
			} else {
				// do nothing.
			}
			crk_list.num--;
		}
	}

	return 0;
}
