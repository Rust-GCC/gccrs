// PR c++/118630

namespace test1 {
  template <typename> decltype([]{}) a();
}

namespace test2 {
  template <typename T> struct invoke_result {};
  template <typename T> struct foo {};

  template <typename T>
  auto b(T&& arg) -> foo<invoke_result<decltype(arg)>> {
    invoke_result<decltype(arg)> a;
    return {};
  }
}
