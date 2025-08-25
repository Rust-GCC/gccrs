struct Foo { x: isize }
extern {
    fn foo(1: ());
// { dg-error ".E0130." "" { target *-*-* } .-1 }
    fn bar((): isize);
// { dg-error ".E0130." "" { target *-*-* } .-1 }
    fn baz(Foo { x }: isize);
// { dg-error ".E0130." "" { target *-*-* } .-1 }
    fn qux((x,y): ());
// { dg-error ".E0130." "" { target *-*-* } .-1 }
    fn this_is_actually_ok(a: usize);
    fn and_so_is_this(_: usize);
}
fn main() {}

