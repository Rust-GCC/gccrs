// compile-flags: -Z chalk

trait Foo { }
impl Foo for i32 { }

trait Bar { }
impl Bar for i32 { }
impl Bar for u32 { }

fn only_foo<T: Foo>(_x: T) { }

fn only_bar<T: Bar>(_x: T) { }

fn main() {
    let x = 5.0;

    // The only type which implements `Foo` is `i32`, so the chalk trait solver
    // is expecting a variable of type `i32`. This behavior differs from the
    // old-style trait solver. I guess this will change, that's why I'm
    // adding that test.
    // FIXME(chalk): order of these two errors is non-deterministic,
    // so let's just hide one for now
    //only_foo(x); // ERROR the trait bound `f64: Foo` is not satisfied

    // Here we have two solutions so we get back the behavior of the old-style
    // trait solver.
    only_bar(x); // { dg-error ".E0277." "" { target *-*-* } }
}

