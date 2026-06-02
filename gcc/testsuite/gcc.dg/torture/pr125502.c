/* { dg-do compile } */

int f() __attribute__((returns_twice));
typedef short v16i16 __attribute__((vector_size(32)));
void f3(short);
void f4()
{
    v16i16 vec6, vec11;
    short v13;
    vec6[0] = f();
    vec11 = vec6;
    f3(v13);
    v13 = vec11[2];
    f4();
}
