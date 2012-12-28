
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <time.h>

#define LINE_SIZE	2048

static char * Mon[] = {
	"Jan", "Feb", "Mar", "Apr",
	"May", "Jun", "Jul", "Aug",
	"Sep", "Oct", "Nov", "Dec",
	NULL
};

static int str2time(char *time_str, time_t *out_time)
{
	int i = 0;
	int mon = 0;
	char *p = NULL;
	struct tm tm1;
	char d[20] = { 0 };

	p = time_str;
	while (Mon[mon] != NULL) {
		if (strncasecmp(Mon[mon], p, 3) == 0) { break; }
		mon++;
	}

	mon++; //mon start from 0, so after matching, it should increase by 1.
	if (mon > 12) { return -1; }
	// the time will be used as interval by relatively, not absolutely, 
	// so the year is not critical in processing.
	sprintf(d, "%d-%2d-", 2012, mon); 
	p += 3;
	i = strlen(d);
	// copy day
	while (' ' == *p) { p++; }
	while (' ' != *p) {
		d[i++] = *p++;
	}
	d[i++] = ' ';
	// copy time 00:00:00
	while (' ' == *p) { p++; }
	while (' ' != *p) {
		d[i++] = *p++;
	}
	d[i] = '\0';
	
	//printf("%s\n", d);
	strptime(d, "%Y-%m-%d %H:%M:%S", &tm1);
	*out_time = mktime(&tm1);

	strftime(d, 20, "%Y-%m-%d %H:%M:%S", localtime(out_time));
	//printf("%s\n", d);

	return 0;
}

/*
 * usage:  secure_log
 *   input: data from /var/log/secure
 *   output: timestamp username ip
 */
int main(int argc, char *argv[])
{
	int i = 0;
	char *p = NULL;
	char line[LINE_SIZE] = { 0 };
	char user[64] = { 0 };
	char ip[24] = { 0 };
	time_t log_time = 0;

	// Dec 21 00:26:35 41886252 sshd[30559]: Failed password for root from 60.174.109.133 port 26505 ssh2
	// Dec  3 17:16:06 41886252 sshd[9275]: Failed password for invalid user test from 203.217.144.17 port 42180 ssh2
	while (fgets(line, LINE_SIZE, stdin)) {
		p = strstr(line, "Failed password for invalid user ");
		if (!p) {
			p = strstr(line, "Failed password for ");
			if (!p) {
				continue;
			} else {
				p += strlen("Failed password for ");
			}
		} else {
			p += strlen("Failed password for invalid user ");
		}

		// get user
		i = 0;
		while (' ' == *p) { p++; }
		while (' ' != *p) {
			user[i++] = *p++;
		}
		user[i] = '\0';

		while (' ' == *p) { p++; }
		while (' ' != *p) { p++; }

		// get ip
		i = 0;
		while (' ' == *p) { p++; }
		while (' ' != *p) {
			ip[i++] = *p++;
		}
		ip[i] = '\0';
		
		str2time(line, &log_time);
		
		fprintf(stdout, "%ld %s %s\n", log_time, user, ip);
		fflush(stdout);
	}

	return 0;
}
