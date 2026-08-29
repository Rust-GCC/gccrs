fn foo(_: impl FnOnce(&u32) -> &u32) {
}

fn baz(_: impl FnOnce(&u32, u32) -> &u32) {
}

fn bar() {
    let x = 22;
    foo(|a| &x)
// { dg-error ".E0597." "" { target *-*-* } .-1 }
}

fn foobar() {
    let y = 22;
    baz(|first, second| &y)
// { dg-error ".E0597." "" { target *-*-* } .-1 }
}

fn main() { }

