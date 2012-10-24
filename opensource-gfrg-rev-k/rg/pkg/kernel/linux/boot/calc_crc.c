#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

typedef unsigned long checksum_t;

#define ERROR(format, ...) \
    do { \
	fprintf(stderr, format "\n", ##__VA_ARGS__); \
	exit(2); \
    } while (0)

#define BUF_SIZE (4 * 1024)

int main(int argc, char **argv)
{
    int fd;
    checksum_t checksum = 0;
    int nbytes;
    unsigned char buf[BUF_SIZE];

    if (argc != 2 || (argv[1][0] == '-' &&
	(strcmp(argv[1], "-h") || strcmp(argv[1], "--help"))))
    {
	fprintf(stderr, "Usage: %s [options] <file>\n"
	    "   Calculates crc of a given file by summarizing all its bytes.\n"
	    "\n   Options:\n"
	    " -h|--help - view this text.\n",
	    argv[0]);
	return (argc == 2) ? 0 : 1;
    }

    if ((fd = open(argv[1], O_RDONLY)) < 0)
	ERROR("Cannot open file `%s'", argv[1]);

    while ((nbytes = read(fd, buf, BUF_SIZE)) > 0)
    {
	unsigned char *p = buf;

	while (nbytes--)
	    checksum += *(p++);
    }

    if (nbytes < 0)
	ERROR("Error occured during reading file `%s'", argv[1]);

    close(fd);
    printf ("%lX\n", checksum);
    return 0;
}
