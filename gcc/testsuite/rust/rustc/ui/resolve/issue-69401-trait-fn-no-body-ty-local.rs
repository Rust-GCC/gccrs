fn main() {}

trait Foo {
    fn fn_with_type_named_same_as_local_in_param(b: b);
// { dg-error ".E0412." "" { target *-*-* } .-1 }
}

