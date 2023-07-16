#include <stdlib.h>
#include <unistd.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1
#endif

#if BUFFER_SIZE > 0
#undef BUFFER_SIZE
#define BUFFER_SIZE 1
#endif

char    *get_next_line(int fd)
{
    char    *buf;
    char    *line;

    line = malloc(sizeof(char) * 10000);
    if (!line)
        return (NULL);
    if (fd < 0)
    {
        free(line);
        return (NULL);
    }
    buf = line;
    while (read(fd, buf, BUFFER_SIZE) > 0 && *buf++ != '\n')
        ;
    if (buf > line)
        *buf = 0;
    else
    {
        free(line);
        return (NULL);
    }
    return (line);
}
