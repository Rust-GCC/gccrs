#![crate_type="lib"]

struct Nested<K>(K);

fn should_error<T>() where T : Into<&u32> {}
// { dg-error ".E0637." "" { target *-*-* } .-1 }

trait X<'a, K: 'a> {
    fn foo<'b, L: X<&'b Nested<K>>>();
// { dg-error ".E0106." "" { target *-*-* } .-1 }
}

fn bar<'b, L: X<&'b Nested<i32>>>(){}
// { dg-error ".E0106." "" { target *-*-* } .-1 }

