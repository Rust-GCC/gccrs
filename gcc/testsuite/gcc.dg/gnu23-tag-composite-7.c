/* { dg-do compile } */
/* { dg-options "-std=gnu23" } */

#define NEST(...) typeof(*({ __VA_ARGS__ tmp = { }; &tmp; }))

int e(int n)
{
    struct foo { char buf[n]; } *p;
    {
	    struct foo { char buf[n]; } *q;
	    1 ? p : q;
    }
}

int f(int n)
{
    typedef struct foo bar;
    struct foo { NEST(struct foo { bar *x; char buf[n]; }) *x; char buf[n]; } *q;
    typeof(q->x) p0;
    typeof(q->x) p1;
    1 ? p0 : q;
    1 ? p1 : q;
    1 ? p0 : p1;
}

int g(int n)
{
    typedef struct fo2 bar;
    struct fo2 { NEST(struct fo2 { NEST(struct fo2 { bar *x; char buf[n]; }) * x; char buf[n]; }) *x; char buf[n]; } *q;
    typeof(q->x) p0;
    typeof(q->x->x) p1;
    typeof(q->x->x->x) p2;
    1 ? p0 : q;
    1 ? p1 : q;
    1 ? p2 : q;
    1 ? p0 : p1;
    1 ? p2 : p1;
    1 ? p0 : p2;
}

int h0(int n)
{
    typedef struct foo bar;
    struct foo { NEST(struct foo { bar *x; char buf[3]; }) *x; char buf[n]; } *q;
    typeof(q->x) p0;
    typeof(q->x) p1;
    1 ? p0 : q;
    1 ? p1 : q;
    1 ? p0 : p1;
}

int h1(int n)
{
    typedef struct foo bar;
    struct foo { NEST(struct foo { bar *x; char buf[n]; }) *x; char buf[3]; } *q;
    typeof(q->x) p0;
    typeof(q->x) p1;
    1 ? p0 : q;
    1 ? p1 : q;
    1 ? p0 : p1;
}

int h2(int n)
{
    typedef struct foo bar;
    struct foo { NEST(struct foo { bar *x; char buf[n]; }) *x; char buf[ ]; } *q;
    typeof(q->x) p0;
    typeof(q->x) p1;
    1 ? p0 : q;
    1 ? p1 : q;
    1 ? p0 : p1;
}

