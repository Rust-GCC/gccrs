/* { dg-do compile } */
/* { dg-options "-std=gnu23" } */

void f1(int n)
{
    struct foo b();
    struct foo {
        char buf[n];
    };
    goto out;		/* { dg-error "jump into scope" } */
    typeof(b) *t;
out:
}

void f2(int n)
{
    struct foo *b[1];
    struct foo {
        char buf[n];
    };
    goto out;		/* { dg-error "jump into scope" } */
    typeof(b) *t;
out:
}

void f3(int n)
{
    struct foo (*b[1])();
    struct foo {
        char buf[n];
    };
    goto out;		/* { dg-error "jump into scope" } */
    typeof(b) *t;
out:
}


void f4(int n)
{
    const struct foo * const * const c;
    struct foo {
        char buf[n];
    };
    goto out;		/* { dg-error "jump into scope" } */
    typeof(c) *t;
out:
}

