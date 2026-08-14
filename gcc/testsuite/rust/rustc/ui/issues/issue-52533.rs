fn foo(_: impl for<'a> FnOnce(&'a u32, &u32) -> &'a u32) {
}

fn main() {
    foo(|a, b| b)
// { dg-error ".E0312." "" { target *-*-* } .-1 }
}

