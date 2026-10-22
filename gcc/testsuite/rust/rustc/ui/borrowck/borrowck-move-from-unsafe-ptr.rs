unsafe fn foo(x: *const Box<isize>) -> Box<isize> {
    let y = *x; // { dg-error ".E0507." "" { target *-*-* } }
    return y;
}

fn main() {
}

