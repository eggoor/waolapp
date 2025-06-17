#include "helper.h"

char* str_16_to_8(const wchar_t* const restrict_ wstr)
{
    char* str = NULL;

    if (!wstr) {
        goto end;
    }

    int buf_siz = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);

    if (!buf_siz) {
        goto end;
    }

    str = malloc(buf_siz);

    if (!str) {
        goto end;
    }

    buf_siz = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, buf_siz, NULL, NULL);

    if (0 == buf_siz) {
        free(str);
        str = NULL;
    }

end:
    return str;
}
