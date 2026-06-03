// PR c++/125575
// { dg-do compile { target c++14 } }

template<typename T>
constexpr T v1 = 42;
template const int v1<int>;

template<typename T>
constexpr T v2 = 42;
template int v2<int>; // { dg-error "type .int. for explicit instantiation .v2. does not match declared type .const int." }

template<typename T>
constexpr T v3 = 42;
template int *v3<int>; // { dg-error "type .int\\*. for explicit instantiation .v3. does not match declared type .const int." }

template<typename T>
constexpr const T *const *v4 = nullptr;
template int *const *v4<int>; // { dg-error "type .int\\* const\\*. for explicit instantiation .v4. does not match declared type .const int\\* const\\* const." }

template<typename T>
constexpr T v5 = 42;
template float v5<int>; // { dg-error "type .float. for explicit instantiation .v5. does not match declared type .const int." }

template<typename T>
T v6 = 42;
template volatile int v6<int>; // { dg-error "type .volatile int. for explicit instantiation .v6. does not match declared type .int." }

template<typename>
constexpr auto v7 = 42;
template int v7<int>; // { dg-error "type .int. for explicit instantiation .v7. does not match declared type .const int." }

template<typename>
auto v8 = 42;
template const int v8<int>; // { dg-error "type .const int. for explicit instantiation .v8. does not match declared type .int." }

template<typename T>
T v9{};
template int v9<int[10]>[5]; // { dg-error "type .int \\\[5\\\]. for explicit instantiation .v9. does not match declared type .int \\\[10\\\]." }

template<typename T>
T v10{};
template int v10<int[10]>[10];

template<typename T>
T v11{};
template int v11<int[10][10]>[10][10];

template<typename T>
T v12{};
template int v12<int[10][10]>[][10];

template<typename T> int arr[sizeof(T)] = {};
template int arr<int>[];
