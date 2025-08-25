// ignore-arm stdcall isn't supported

fn baz(f: extern "stdcall" fn(usize, ...)) {
// { dg-error ".E0045." "" { target *-*-* } .-1 }
    f(22, 44);
}

fn main() {}

