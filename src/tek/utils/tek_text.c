#include "tek_text.h"
#include "tek_types.h"

#include <string.h>
#include <stdlib.h>

char *get_file_extension(const char *filename)
{
    size_t len = strlen(filename);

    //TODO: check for overflow
    char *ext = malloc(sizeof(char) * 8);
    u32 id = 0;
    u32 ctr = 0;

    for (int i = len - 1; i >= 0; --i) 
    {
        char c = filename[i];
        if (c == '.') {
            id = i;
            break;
        }
    }

    for (u32 i = id + 1; i < len; ++i) {
        ext[ctr++] = filename[i];
    }
    ext[ctr] = '\0';

    return ext;
}