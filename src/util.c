#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


char *
read_file(const char *path)
{
    char   *buffer = NULL;
    FILE   *stream = NULL;
    long    end;
    size_t  size;

    stream = fopen(path, "rb");
    assert(stream != NULL);

    assert(fseek(stream, 0, SEEK_END) == 0);

    end = ftell(stream);
    assert(end != -1);

    size = (size_t)end;
    buffer = calloc(1, size + 1);
    assert(buffer != NULL);

    rewind(stream);
    assert(fread(buffer, 1, size, stream) == size);

    assert(fclose(stream) == 0);

    return buffer;
}
