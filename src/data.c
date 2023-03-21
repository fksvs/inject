#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <sys/stat.h>
#include <fcntl.h>

#include "error_func.h"
#include "data.h"

char *read_file(char *file_name)
{
	char *payload;
	int fd;
	struct stat st;

	if ((fd = open(file_name, O_RDONLY)) == -1) {
		err_msg("data.c", "read_file", __LINE__, errno);
		return NULL;
	}
	if (fstat(fd, &st) == -1) {
		err_msg("data.c", "read_file", __LINE__, errno);
		return NULL;
	}

	payload = malloc(st.st_size);
	memset(payload, 0, st.st_size);

	if (read(fd, payload, st.st_size) == -1) {
		err_msg("data.c", "read_file", __LINE__, errno);
		return NULL;
	}

	close(fd);
	return payload;
}
