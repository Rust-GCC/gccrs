// Test a case where you have an impl of `Foo<X>` for all `X` that
// is being applied to `for<'a> Foo<&'a mut X>`. Issue #19730.

trait Foo<X> {
    fn foo(&self, x: X) { }
}

fn want_hrtb<T>()
    where T : for<'a> Foo<&'a isize>
{
}

// AnyInt implements Foo<&'a isize> for any 'a, so it is a match.
struct AnyInt;
impl<'a> Foo<&'a isize> for AnyInt { }
fn give_any() {
    want_hrtb::<AnyInt>()
}

// StaticInt only implements Foo<&'static isize>, so it is an error.
struct StaticInt;
impl Foo<&'static isize> for StaticInt { }
fn give_static() {
    want_hrtb::<StaticInt>() // { dg-error "" "" { target *-*-* } }
}

// AnyInt implements Foo<&'a isize> for any 'a, so it is a match.
impl<'a> Foo<&'a isize> for &'a u32 { }
fn give_some<'a>() {
    want_hrtb::<&'a u32>() // { dg-error "" "" { target *-*-* } }
}

fn main() { }

