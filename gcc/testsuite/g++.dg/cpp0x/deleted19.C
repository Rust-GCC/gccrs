// PR c++/120338
// { dg-do compile { target c++11 } }

int main() = delete;  // { dg-error "cannot be deleted" }
int main() {}	      // { dg-error "redefinition" }
