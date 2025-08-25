struct Foo {
}

fn foo(&foo: Foo) { // { dg-error ".E0308." "" { target *-*-* } }
}

fn bar(foo: Foo) {
}

fn qux(foo: &Foo) {
}

fn zar(&foo: &Foo) {
}

// The somewhat unexpected help message in this case is courtesy of
// match_default_bindings.
fn agh(&&bar: &u32) { // { dg-error ".E0308." "" { target *-*-* } }
}

fn bgh(&&bar: u32) { // { dg-error ".E0308." "" { target *-*-* } }
}

fn ugh(&[bar]: &u32) { // { dg-error ".E0529." "" { target *-*-* } }
}

fn main() {}

