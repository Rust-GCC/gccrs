// PR target/124597
// { dg-do compile { target lp64 } }

// Make sure va_end usage works with exception branch.
struct a {
  a(int);
  ~a();
};
int b;
__builtin_ms_va_list c;
void d() {
  a e(b);
  __builtin_ms_va_end(c);
}
