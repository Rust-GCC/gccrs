/* { dg-do run } */
/* { dg-options "-std=gnu23 -O2" } */


int k;
struct S {};

#define T struct S

T *get_sender () {
  k = 1;
  return (void *)0;
}

T *get_cc () { }

void test () {
        struct {
                T *(*get_header) (void);
        } reply_to_map[] = {
                { get_sender },
                { get_cc },
        };

        for (int i = 0; i < 2; i++) {
                asm ("" ::"r"(reply_to_map[i].get_header ()));
        }
}

int main() {
        test ();

        if (k != 1)
            __builtin_abort();
}

