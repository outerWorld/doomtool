
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

// input: short-string, max number of chars is 8
// output: 64bit-number hexical string
// usage: schar2int short-string
int main(int argc, char *argv[])
{
	unsigned long long u64_value = 0;
	unsigned char buf[12] = { 0 };
	
	if (argc < 2) return 0;

	strncpy(buf, argv[1], 8);
	buf[8] = '\0';

	strto64(buf, strlen(buf), 0);

	return 0;
}
