fn foo(_x: isize) { }

fn main() {
    let v: u64 = 5;
    let x = foo as extern "C" fn() -> isize;
// { dg-error ".E0605." "" { target *-*-* } .-1 }
    let y = v as extern "Rust" fn(isize) -> (isize, isize);
// { dg-error ".E0605." "" { target *-*-* } .-1 }
    y(x());
}

