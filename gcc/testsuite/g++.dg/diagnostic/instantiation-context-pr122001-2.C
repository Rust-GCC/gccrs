// { dg-additional-options "-fno-diagnostics-show-nesting" }

template <typename T>
void foo(T t) {
  1 + t; // { dg-error "pointer-arith" }
}

int main() {
  foo((void *)0);
}

// Verify that we print the file/line/column before the "required from here"
// { dg-regexp ".*instantiation-context-pr122001-2.C:9:6:   required from here" }
