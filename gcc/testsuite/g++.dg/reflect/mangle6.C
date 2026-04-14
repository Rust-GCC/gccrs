// PR c++/124824
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

template<auto V> void f() { }

int main() {
  {
    static constexpr auto M = ^^int;
    f<&M>();
  }
  {
    static constexpr auto M = ^^int;
    f<&M>();
  }
}

// { dg-final { scan-assembler "_Z1fITnDaXadL_ZZ4mainE1MEEEvv" } }
// { dg-final { scan-assembler "_Z1fITnDaXadL_ZZ4mainE1M_0EEEvv" } }
