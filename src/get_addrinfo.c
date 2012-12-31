
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

static void print_addrinfo(struct addrinfo * ai)
{
	char _ip[128] = { 0 };
	struct sockaddr_in * addr;
	fprintf(stdout, "flags = %d, family = %d, socktype = %d, protocol = %d\n",
			ai->ai_flags, ai->ai_family, ai->ai_socktype, ai->ai_protocol);
	addr = (struct sockaddr_in *)ai->ai_addr;
	if (ai->ai_addr) {
		if (NULL == inet_ntop(addr->sin_family, (void*)&addr->sin_addr, _ip, 128)) {
			fprintf(stderr, "error calling inet_ntop [%s]\n", strerror(errno));
			return;
		}
		fprintf(stdout, "sockaddr_size = %d, sockaddr = [%s, %d, %d], cononame = %s\n", 
				ai->ai_addrlen, _ip,  ntohs(addr->sin_port), addr->sin_family, ai->ai_canonname);
	}
}

/*
 * test_addrinfo ip port
 */
int main(int argc, char *argv[])
{
	struct addrinfo hint_ai, *serv_ai, *p;
	char * ip;
	char *port;

	if (argc < 3) {
		return 1;
	}

	ip = argv[1];
	port = argv[2];

	memset(&hint_ai, 0x00, sizeof(struct addrinfo));
	hint_ai.ai_family = AF_INET;
	hint_ai.ai_socktype = SOCK_STREAM;

	if (getaddrinfo(ip, port, &hint_ai, &serv_ai) != 0) {
		fprintf(stderr, "error calling getaddrinfo [%s]\n", (char*)gai_strerror(errno));
		return 1;
	}

	p = serv_ai;
	while (p) {
		print_addrinfo(p);
		p = p->ai_next;			
	}

	freeaddrinfo(serv_ai);
	
	return 0;
}
