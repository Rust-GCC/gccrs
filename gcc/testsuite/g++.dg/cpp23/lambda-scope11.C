// PR c++/122632
// { dg-do compile { target c++20 } }

void f(auto... x) {
  [...y = x]() noexcept((... && noexcept(y))) {};
}

int main() {
	f();
}
