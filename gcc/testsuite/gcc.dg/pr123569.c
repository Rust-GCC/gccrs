/* { dg-do run } */
/* { dg-options "-std=c99" } */

struct buffer {
    int len;
    char * ptr __attribute((counted_by(len)));
};
int ltrim(struct buffer * const buf) {
    while (buf->len > 0 && *buf->ptr == ' ') {
        buf->len--;
        buf->ptr++;
    }
    return buf->len;
}
int main() {
    struct buffer buf = {.ptr = " 123", .len = 4};
    int ret = ltrim(&buf);
    if (3 != ret)
      __builtin_abort();
    return 0;
}

