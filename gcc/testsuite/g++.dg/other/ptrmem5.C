// PR 14123

struct a
{
  int x[8];
};

int main()
{
  int a::*n[8];
  n = &a::x; // { dg-error "5:incompatible types in assignment of .int \\(a::\\*\\)" }
}   
