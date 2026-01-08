#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "fileio.h"
#include "helpers.h"

char *read_file(char *file_name)
{
	char *payload;
	int fd;
	struct stat st;

	if ((fd = open(file_name, O_RDONLY)) == -1) {
		err_msg("fileio.c", "read_file", __LINE__, errno);
		return NULL;
	}
	if (fstat(fd, &st) == -1) {
		err_msg("fileio.c", "read_file", __LINE__, errno);
		close(fd);
		return NULL;
	}

	/* Allocate buffer with extra byte for null terminator (file content is used as string) */
	payload = malloc(st.st_size + 1);
	if (payload == NULL) {
		err_msg("fileio.c", "read_file", __LINE__, errno);
		close(fd);
		return NULL;
	}
	memset(payload, 0, st.st_size + 1);

	if (read(fd, payload, st.st_size) == -1) {
		err_msg("fileio.c", "read_file", __LINE__, errno);
		free(payload);
		close(fd);
		return NULL;
	}

	close(fd);
	return payload;
}
