#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define T unsigned

void fast_sort(T *begin, T *end) {
    size_t *count = (size_t *) malloc(256 * sizeof(size_t));
    if (!count) {
        return;
    }
    T *buff = (T *) malloc((end - begin) * sizeof(T));
    if (!buff) {
        free(count);
        return;
    }

    T mask = 255;
    for (size_t i = 0; i < sizeof(T); ++i) {
        for (size_t j = 0; j < 256; ++j) {
            count[j] = 0;
        }

        for (T *curr = begin; curr < end; ++curr) {
            ++count[(*curr & mask) >> (i << 3)];
        }
        --count[0];
        for (size_t j = 1; j < 256; ++j) {
            count[j] += count[j - 1];
        }
        for (T *curr = end - 1; curr >= begin; --curr) {
            T tmp = *curr;
            size_t ind = (tmp & mask) >> (i << 3);
            buff[count[ind]] = tmp;
            --count[ind];
        }
        memcpy(begin, buff, (end - begin) * sizeof(T));
        mask <<= 8;
    }

    free(count);
    free(buff);
}