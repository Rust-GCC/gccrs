// Check that taking the address of an argument yields a lifetime
// bounded by the current function call.

fn foo(a: isize) {
    let _p: &'static isize = &a; // { dg-error ".E0597." "" { target *-*-* } }
}

fn bar(a: isize) {
    let _q: &isize = &a;
}

fn zed<'a>(a: isize) -> &'a isize {
    &a // { dg-error ".E0515." "" { target *-*-* } }
}

fn main() {
}

