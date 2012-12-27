
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>

typedef struct buffer_s {
	int 	size;
	int		len;
	char * 	buf;
}buffer_t, *buffer_p;

static int enlarge_buffer(buffer_p buf_obj, int size)
{
	char * tmp_buf = NULL;

	tmp_buf = buf_obj->buf;
	if (size <= buf_obj->size) {
		size = (size+1)*2;
	}

	buf_obj->buf = (char*)malloc(size);
	if (!buf_obj->buf) {
		buf_obj->buf = tmp_buf;
		return -1;
	}

	if (buf_obj->len > 0) { memcpy(buf_obj->buf, tmp_buf, buf_obj->len); }

	if (tmp_buf) { free(tmp_buf); }
	buf_obj->size = size;
	
	return 0;
}

static int free_buffer(buffer_p buf_obj)
{
	if (buf_obj) {
		if (buf_obj->buf) { free(buf_obj->buf); }
		memset(buf_obj, 0x00, sizeof(buffer_t));
	}

	return 0;
}

enum {
	ERR_CONTINUE = 0x00,
};
static int check_read_error(int err)
{

	return 0;
}

static int line_filters(int out_fd, buffer_p buf, char *filter)
{
	char * pb = NULL;
	char * pe = NULL;

	pb = buf->buf;
	pe = strstr(pb, "\n");
	while (pe) {
		*pe = '\0';
		if (strstr(pb, filter)) {
			// write --> writev
			if (write(out_fd, pb, strlen(pb))) { }
			if (write(out_fd, "\n", 1)) { }
		}
		*pe = '\n';
		pb = pe + 1;
		pe = strstr(pb, "\n");
	}
}

/*
 * usage: fmore secure.log filters
 */
int main(int argc, char *argv[])
{
	int i = 0;
	int fd = 0;
	ssize_t len = 0;
	buffer_t buf;
	off_t cur_off = 0;
	off_t next_off = 0;
	ssize_t read_len = 0;
	char filter[512] = { 0 };
	
	if (argc < 2) {
		return 1;
	}

	for (i=2; i<argc; i++) {
		strcat(filter, argv[i]);
		strcat(filter, " ");
	}
	if (strlen(filter) > 0) {
		filter[strlen(filter)-1] = '\0';
	}

	memset(&buf, 0x00, sizeof(buffer_t));
	fd = open(argv[1], O_RDONLY, 0);
	if (fd <= 0) {
		fprintf(stderr, "open %s fail! [%d:%s]\n", argv[1], errno, strerror(errno));
		return 1;
	}

	cur_off = lseek(fd, 0, SEEK_SET);
	while (1) {
		next_off = lseek(fd, 0, SEEK_END);
		len = next_off - cur_off;
		if (len > buf.size) { enlarge_buffer(&buf, len); }

		if (len > 0) {
			lseek(fd, cur_off, SEEK_SET);
			read_len = read(fd, buf.buf, len);
			if (-1 == read_len) {
				if (check_read_error(errno) != ERR_CONTINUE) { break; }
			}
			buf.len = read_len;
			//fprintf(stdout, "%s", buf.buf);
			buf.buf[buf.len] = '\0';
			if (line_filters(1, &buf, filter) < 0) {
			}
		}
		
		buf.len = 0;
		cur_off = next_off;
		usleep(10);
	}

	close(fd);
	free_buffer(&buf);

	return 0;
}
