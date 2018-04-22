#include <iconv.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "utils.h"

uint8_t read_uint8(FILE *fd) {
    uint8_t buf[1];
    fread(&buf, 1, 1, fd);
    return buf[0];
}

uint16_t read_uint16_littleendian(FILE *fd) {
    uint8_t buf[2];
    fread(&buf, 1, 2, fd);
    return (uint16_t) (buf[1] << 8 | buf[0]);
}
uint32_t read_uint32_littleendian(FILE *fd) {
    uint8_t buf[4];
    fread(&buf, 1, 4, fd);
    return (uint32_t) (buf[3] << 24 | buf[2] << 16 | buf[1] << 8 | buf[0]);
}

uint16_t read_uint16_bigendian(FILE *fd) {
    uint8_t buf[2];
    fread(&buf, 1, 2, fd);
    return (uint16_t) (buf[0] << 8 | buf[1]);
}
uint32_t read_uint32_bigendian(FILE *fd) {
    uint8_t buf[4];
    fread(&buf, 1, 4, fd);
    return (uint32_t) (buf[0] << 24 | buf[1] << 16 | buf[2] << 8 | buf[3]);
}

char* utf16_to_utf8(const char *str, size_t size) {
    /* On alloue un buffer deux fois plus gros que l'entrée. Dans la
     * plupart des cas c'est trop ; mais c'est le moyen le plus simple
     * de s'assurer qu'on ne fait pas de dépassement de buffer
     * dans cette situation. */
    char *outbuf = malloc(size * 2);
    assert(outbuf);

    /* FIXME: useless copy, because iconv takes a char** as inbuf instead
     * of const char**. */
    char *inbuf = malloc(size);
    assert(inbuf);
    memcpy(inbuf, str, size);

    size_t inbytesleft=size, outbytesleft=size*2;
    iconv_t conv = iconv_open("UTF-8", "UTF-16LE");

    char *inbuf_it = inbuf;
    char *outbuf_it = outbuf;
    while (inbytesleft > 0) {
        size_t res = iconv(conv, &inbuf_it, &inbytesleft, &outbuf_it, &outbytesleft);
        if (res == (size_t) -1) {
            perror("Error converting from UTF16 to UTF8");
            exit(EXIT_FAILURE);
        }
    }
    iconv_close(conv);
    free(inbuf);
    return outbuf;
}

