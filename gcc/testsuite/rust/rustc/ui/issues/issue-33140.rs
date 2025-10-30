trait Trait {
    fn xyz() -> bool;
}

impl Trait for dyn Send + Sync {
    fn xyz() -> bool { false }
}

impl Trait for dyn Sync + Send {
// { dg-error ".E0119." "" { target *-*-* } .-1 }
    fn xyz() -> bool { true }
}

trait Trait2 {
    fn uvw() -> bool;
}

impl Trait2 for dyn Send + Sync {
    fn uvw() -> bool { false }
}

impl Trait2 for dyn Sync + Send + Sync {
// { dg-error ".E0119." "" { target *-*-* } .-1 }
    fn uvw() -> bool { true }
}

struct Foo<T: ?Sized>(T);
impl Foo<dyn Send + Sync> {
    fn abc() -> bool { // { dg-error ".E0592." "" { target *-*-* } }
        false
    }
}

impl Foo<dyn Sync + Send> {
    fn abc() -> bool {
        true
    }
}

fn main() {
    assert_eq!(<dyn Send + Sync>::xyz(), false);
    assert_eq!(<dyn Sync + Send>::xyz(), true);
    assert_eq!(<dyn Send + Sync>::uvw(), false);
    assert_eq!(<dyn Sync + Send+ Sync>::uvw(), true);
    assert_eq!(<Foo<dyn Send + Sync>>::abc(), false);
    assert_eq!(<Foo<dyn Sync + Send>>::abc(), true);
}

