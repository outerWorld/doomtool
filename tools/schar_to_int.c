
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static unsigned long long _u64(char *data, int len)
{
	unsigned long long _value = 0;
	int i = 0;

	for (i=len-1; i>=0; i--) {
		_value = _value << 8;
		_value |= data[i];
	}

	return _value;
}

int strto64(unsigned char *data, int len, int pos)
{
	if (pos >= len) {
		fprintf(stdout, "%.*s = 0x%llx\n", len, data, _u64(data, len));
	} else {
		strto64(data, len, pos+1);
		if (isalpha(data[pos])) {
			if (isupper(data[pos]))
				data[pos] += 0x20;
			else
				data[pos] -= 0x20;
			strto64(data, len, pos+1);
		}
	}

	return 0;
}

// usage: schar2int type short-string
// example: schar2int 0 html
// input: short-string, max number of chars is 8
//		  type, 0(read short-string from commandline), 1(read short-string from stdin)
// output: 64bit-number hexical string
int main(int argc, char *argv[])
{
	unsigned long long u64_value = 0;
	unsigned char buf[12] = { 0 };
	char line[1024] = { 0 };
	int type = 0;
	char *p;
	
	if (argc < 2) return 0;

	type = atoi(argv[1]);
	
	if (0 == type) {
		if (argc < 3) return 0;
		strncpy(buf, argv[2], 8);
		buf[8] = '\0';
		strto64(buf, strlen(buf), 0);
		return 0;
	}

	while (fgets(line, 1024, stdin)) {
		line[strlen(line)-1] = '\0';
		p = strtok(line, " ");
		while (p) {
			fprintf(stdout, "string_data = %s\n", p);
			if (strlen(p) <= 8) {
				strcpy(buf, p);
				strto64(buf, strlen(buf), 0);
			}
			p = strtok(NULL, " ");
		}
	}

	return 0;
}
