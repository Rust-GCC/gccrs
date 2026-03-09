// PR c++/123440
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>
#include <ranges>

using namespace std::meta;

struct test_struct
{
    int data_member;
    void fn1() {}
    void fn2() {}
    int getter() { return data_member; }
    void setter(int i) { data_member = i; }
    virtual int virtual_fn(char*, bool) { return 0; }
    virtual int pure_virtual_fn(char*, int) = 0;
    static int static_member_fn(bool b) { return b ? 42 : 0; }
};
template<typename T, size_t I>
consteval info get_member_at()
{
    constexpr auto ctx          = access_context::current();
    constexpr auto pointable    = [](info i) consteval -> bool {
        return !is_destructor(i) && !is_constructor(i) && !is_special_member_function(i);
    };
    return (members_of(^^T, ctx) | std::views::filter(pointable) | std::ranges::to<std::vector>())[I];
}
template<typename T, size_t I>
consteval info get_member_type() {
    return type_of(get_member_at<T, I>());
}
template<typename T, size_t I>
consteval auto get_member_ptr() {
    constexpr info member   = get_member_at<T, I>();
    constexpr info mem_type = add_pointer(get_member_type<T, I>());
    using type              = typename[: mem_type :];
    return extract<type>(member);
}
constexpr auto ex_0 = extract<int test_struct::*>(get_member_at<test_struct, 0>());
constexpr auto ex_1 = extract<void (test_struct::*)()>(get_member_at<test_struct, 1>());
constexpr auto ex_2 = extract<void (test_struct::*)()>(get_member_at<test_struct, 2>());
constexpr auto ex_3 = extract<int (test_struct::*)()>(get_member_at<test_struct, 3>());
constexpr auto ex_4 = extract<void (test_struct::*)(int)>(get_member_at<test_struct, 4>());
constexpr auto ex_5 = extract<int (test_struct::*)(char*, bool)>(get_member_at<test_struct, 5>());
constexpr auto ex_6 = extract<int (test_struct::*)(char*, int)>(get_member_at<test_struct, 6>());
constexpr auto ex_7 = extract<int(*)(bool)>(get_member_at<test_struct, 7>());
constexpr auto ex_8 = get_member_ptr<test_struct, 7>();
constexpr auto test_1 = get_member_ptr<test_struct, 1>(); // { dg-error "value cannot be extracted" }
constexpr auto test_2 = get_member_ptr<test_struct, 2>(); // { dg-error "value cannot be extracted" }
constexpr auto test_3 = get_member_ptr<test_struct, 3>(); // { dg-error "value cannot be extracted" }
constexpr auto test_4 = get_member_ptr<test_struct, 4>(); // { dg-error "value cannot be extracted" }
constexpr auto test_5 = get_member_ptr<test_struct, 5>(); // { dg-error "value cannot be extracted" }
constexpr auto test_6 = get_member_ptr<test_struct, 6>(); // { dg-error "value cannot be extracted" }
