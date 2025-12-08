extern fn f() {
}

fn is_fn<F>(_: F) where F: Fn() {}

fn main() {
    // extern functions are extern "C" fn
    let _x: extern "C" fn() = f; // OK
    is_fn(f);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

