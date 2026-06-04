/* { dg-do compile } */
/* { dg-options "-w" } */
/* PR middle-end/125588 */
void f() {
unsigned short a = 8 <= 0 % 0 |
            7 >> (6 >> 48)  % 1  ^
            6 >> 8 <= 0 &
                7 >> (6 >> 48) % 1;
}
