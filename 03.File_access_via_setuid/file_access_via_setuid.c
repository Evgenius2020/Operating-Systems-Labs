#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int try_to_open_file(char *filename)
{
    if (access(filename, F_OK) == -1)
    {
        perror(NULL);
        return ENOENT;
    }

    FILE *file = fopen(filename, "w");
    if (!file)
    {
        perror(NULL);
        return errno;
    }
    printf("Open file: success.\n");
    fclose(file);

    return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s [file]\n", argv[0]);
        return EXIT_FAILURE;
    }

    printf("uid = %d, ueid = %d\n", getuid(), geteuid());

    try_to_open_file(argv[1]);

    if (setuid(geteuid()) == -1)
    {
        fprintf(stderr, "Can't change euid\n");
        perror(NULL);
        return errno;
    }
    printf("Change euid: success.\n");
    printf("uid = %d, ueid = %d\n", getuid(), geteuid());

    try_to_open_file(argv[1]);

    return EXIT_SUCCESS;
}