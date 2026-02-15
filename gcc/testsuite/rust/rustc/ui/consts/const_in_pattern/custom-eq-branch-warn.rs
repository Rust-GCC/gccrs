// check-pass

struct CustomEq;

impl Eq for CustomEq {}
impl PartialEq for CustomEq {
    fn eq(&self, _: &Self) -> bool {
        false
    }
}

#[derive(PartialEq, Eq)]
enum Foo {
    Bar,
    Baz,
    Qux(CustomEq),
}

// We know that `BAR_BAZ` will always be `Foo::Bar` and thus eligible for structural matching, but
// dataflow will be more conservative.
const BAR_BAZ: Foo = if 42 == 42 {
    Foo::Bar
} else {
    Foo::Qux(CustomEq)
};

fn main() {
    match Foo::Qux(CustomEq) {
        BAR_BAZ => panic!(),
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-note "" "" { target *-*-* } .-3 }
// { dg-note "" "" { target *-*-* } .-4 }
        _ => {}
    }
}

