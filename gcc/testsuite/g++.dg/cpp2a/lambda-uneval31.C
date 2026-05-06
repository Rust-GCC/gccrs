// PR c++/118482
// { dg-do compile { target c++20 } }

using what_type = decltype(
    [](auto&& self,
        decltype([](decltype(self)&){}) x = {}){ return x; });

what_type x;

int main() {
    if (x(x)) return 1;
}
