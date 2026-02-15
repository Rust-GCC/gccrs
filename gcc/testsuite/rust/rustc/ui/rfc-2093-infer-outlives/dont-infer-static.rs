/*
 * We don't infer `T: 'static` outlives relationships by default.
 * Instead an additional feature gate `infer_static_outlives_requirements`
 * is required.
 */

struct Foo<U> {
    bar: Bar<U> // { dg-error ".E0310." "" { target *-*-* } }
}
struct Bar<T: 'static> {
    x: T,
}

fn main() {}

