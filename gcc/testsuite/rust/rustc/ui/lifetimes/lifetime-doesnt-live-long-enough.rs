trait ListItem<'a> {
    fn list_name() -> &'a str;
}

trait Collection { fn len(&self) -> usize; }

// is now well formed. RFC 2093
struct List<'a, T: ListItem<'a>> {
    slice: &'a [T]
}

impl<'a, T: ListItem<'a>> Collection for List<'a, T> {
    fn len(&self) -> usize {
        0
    }
}

struct Foo<T> {
    foo: &'static T
// { dg-error ".E0310." "" { target *-*-* } .-1 }
}

trait X<K>: Sized {
    fn foo<'a, L: X<&'a Nested<K>>>();
// { dg-error ".E0309." "" { target *-*-* } .-1 }

    // check that we give a sane error for `Self`
    fn bar<'a, L: X<&'a Nested<Self>>>();
// { dg-error ".E0309." "" { target *-*-* } .-1 }

    // check that we give a sane error for nested generics
    fn baz<'a, L, M: X<&'a Nested<L>>>() {
// { dg-error ".E0309." "" { target *-*-* } .-1 }
    }
}

trait TraitB {}

struct Nested<K>(K);
impl<K> Nested<K> {
    fn generic_in_parent<'a, L: X<&'a Nested<K>>>() {
// { dg-error ".E0309." "" { target *-*-* } .-1 }
    }
    fn generic_in_child<'a, 'b, L: X<&'a Nested<M>>, M: 'b>() {
// { dg-error ".E0309." "" { target *-*-* } .-1 }
    }
}

fn main() {}

