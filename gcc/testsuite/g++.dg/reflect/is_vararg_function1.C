// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }
// Test std::meta::is_vararg_function.

#include <meta>

using namespace std::meta;

constexpr info null_reflection;
struct cls {
  int dm;
  static int static_dm;
  void mem_fun ();
  void mem_fun2 (int, ...);
  void mem_fun3 (...);
  void mem_fun4 (int);
  static void static_mem_fun ();
  static void static_mem_fun2 (int, long, ...);
  static void static_mem_fun3 (...);
  static void static_mem_fun4 (long);
  int &ref_dm = dm;
  using type = int;
} cls_var;
union onion { };
static union { int anon; };
using alias = cls;
void fun ();
void fun2 (int, ...);
void fun3 (...);
void fun4 (int, int);
int var;
int &ref = var;
int &&rref = 42;
int *ptr = &var;
namespace ns {}
namespace ns_alias = ns;
enum Enum { A };
enum class Enum_class { A };
using funt = int ();
using funt2 = int (int, ...);
using funt3 = int (...);
using funt4 = int (int, long);

template<typename> struct incomplete_cls;
template<typename> struct cls_tmpl {};
template<typename> void fun_tmpl ();
template<typename> void fun_tmpl2 (int, ...);
template<typename> void fun_tmpl3 (...);
template<typename> void fun_tmpl4 (long, int);
template<typename> concept conc = requires { true; };
template<typename> int var_tmpl;
template<typename T> using cls_tmpl_alias = cls_tmpl<T>;

int arr[] = { 42 };
using inc_arr = int[];
using com_arr = int[42];
auto [ decomp ] = arr;
auto &[ decomp_ref ] = arr;

static_assert (!is_vararg_function (null_reflection));
static_assert (!is_vararg_function (^^::));
static_assert (!is_vararg_function (^^ns));
static_assert (!is_vararg_function (^^ns_alias));
static_assert (!is_vararg_function (reflect_constant (3)));
static_assert (!is_vararg_function (^^cls));
static_assert (!is_vararg_function (^^cls::dm));
static_assert (!is_vararg_function (^^cls::ref_dm));
static_assert (!is_vararg_function (^^cls::static_dm));
static_assert (!is_vararg_function (^^cls::mem_fun));
static_assert (is_vararg_function (^^cls::mem_fun2));
static_assert (is_vararg_function (^^cls::mem_fun3));
static_assert (!is_vararg_function (^^cls::mem_fun4));
static_assert (!is_vararg_function (^^cls::static_mem_fun));
static_assert (is_vararg_function (^^cls::static_mem_fun2));
static_assert (is_vararg_function (^^cls::static_mem_fun3));
static_assert (!is_vararg_function (^^cls::static_mem_fun4));
static_assert (!is_vararg_function (^^cls::type));
static_assert (!is_vararg_function (^^cls_var));
static_assert (!is_vararg_function (^^onion));
static_assert (!is_vararg_function (^^anon));
static_assert (!is_vararg_function (^^fun));
static_assert (is_vararg_function (^^fun2));
static_assert (is_vararg_function (^^fun3));
static_assert (!is_vararg_function (^^fun4));
static_assert (!is_vararg_function (type_of (^^fun)));
static_assert (is_vararg_function (type_of (^^fun2)));
static_assert (is_vararg_function (type_of (^^fun3)));
static_assert (!is_vararg_function (type_of (^^fun4)));
static_assert (!is_vararg_function (^^alias));
static_assert (!is_vararg_function (^^var));
static_assert (!is_vararg_function (^^ref));
static_assert (!is_vararg_function (^^rref));
static_assert (!is_vararg_function (^^ptr));
static_assert (!is_vararg_function (^^cls_tmpl));
static_assert (!is_vararg_function (^^cls_tmpl<int>));
static_assert (!is_vararg_function (^^incomplete_cls<int>));
static_assert (!is_vararg_function (^^fun_tmpl));
static_assert (!is_vararg_function (^^fun_tmpl<int>));
static_assert (!is_vararg_function (^^fun_tmpl2));
static_assert (is_vararg_function (^^fun_tmpl2<int>));
static_assert (!is_vararg_function (^^fun_tmpl3));
static_assert (is_vararg_function (^^fun_tmpl3<int>));
static_assert (!is_vararg_function (^^fun_tmpl4));
static_assert (!is_vararg_function (^^fun_tmpl4<int>));
static_assert (!is_vararg_function (^^conc));
static_assert (!is_vararg_function (substitute (^^conc, { ^^int })));
static_assert (!is_vararg_function (^^var_tmpl));
static_assert (!is_vararg_function (^^var_tmpl<int>));
static_assert (!is_vararg_function (^^cls_tmpl_alias));
static_assert (!is_vararg_function (^^cls_tmpl_alias<int>));
static_assert (!is_vararg_function (^^Enum));
static_assert (!is_vararg_function (^^Enum::A));
static_assert (!is_vararg_function (^^Enum_class));
static_assert (!is_vararg_function (^^Enum_class::A));
static_assert (!is_vararg_function (^^decomp));
static_assert (!is_vararg_function (^^decomp_ref));
static_assert (!is_vararg_function (^^arr));
static_assert (!is_vararg_function (^^inc_arr));
static_assert (!is_vararg_function (^^com_arr));
static_assert (!is_vararg_function (^^funt));
static_assert (is_vararg_function (^^funt2));
static_assert (is_vararg_function (^^funt3));
static_assert (!is_vararg_function (^^funt4));

constexpr auto dms = data_member_spec (^^int, { .name = u8"a" });
static_assert (!is_vararg_function (dms));

struct Base {};
struct Derived : Base {};
static_assert (!is_vararg_function (bases_of (^^Derived, access_context::unchecked ())[0]));

template<typename T, info R, info R2, info R3>
void
f ()
{
  static_assert (!is_vararg_function (^^T));
  static_assert (!is_vararg_function (R));
  static_assert (!is_vararg_function (R2));
  static_assert (!is_vararg_function (R3));
}

void
g (int p, cls c)
{
  f<int, ^^var, ^^ns, ^^cls>();
  static_assert (!is_vararg_function (^^p));
  static_assert (!is_vararg_function (^^c));
}
