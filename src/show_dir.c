
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>

#include <errno.h>
#include <ftw.h>

#include <dirent.h>
#include <sys/stat.h>

static int file_proc(const char *fpath, const struct stat *st, int typeflag)
{
	fprintf(stdout, "%s, %08x\n", fpath, typeflag);
	return 0;
}
static int use_ftw(const char *dirent)
{
	if (ftw(dirent, file_proc, 20) == -1) {
		fprintf(stderr, "ftw fail [%d,%s]\n", errno, strerror(errno));	
		return -1;
	}

	return 0;
}

static int use_direntry(const char *dirent)
{
	DIR *dp;
	struct dirent *entry;
	struct stat st;
	if (!(dp=opendir(dirent))) {
		fprintf(stderr, "opendir fail [%d,%s]\n", errno, strerror(errno));
		return -1;
	}

	if (0 != chdir(dirent)) {
		closedir(dp);	
		return -1;
	}
	while ((entry=readdir(dp)) != NULL) {
		lstat(entry->d_name, &st);
		if (S_ISDIR(st.st_mode)) {
			fprintf(stdout, "d %s\n", entry->d_name);
			use_direntry(entry->d_name);
		} else {
			fprintf(stdout, "f %s\n", entry->d_name);
		}
	}
	if (0 != chdir(".."))
		exit(1);
	closedir(dp);
}

int main(int argc, char *argv[])
{
	int flags = 0;
	
	if (argc < 2) return 1;

	fprintf(stdout, "use_direntry\n");
	use_direntry(argv[1]);
	fprintf(stdout, "use_ftw\n");
	use_ftw(argv[1]);

	return 0;
}
