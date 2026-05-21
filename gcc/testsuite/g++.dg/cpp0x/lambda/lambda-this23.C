// PR c++/125384
// { dg-do compile { target c++11 } }

template <class,class> struct assert_same;
template <class T> struct assert_same<T,T> {};

template<typename C>
struct At {
  void b() {
    [this](){
      assert_same <decltype (this), At*> as2;
    }();
  }
};

int main()
{
  At<int>{}.b();
}
