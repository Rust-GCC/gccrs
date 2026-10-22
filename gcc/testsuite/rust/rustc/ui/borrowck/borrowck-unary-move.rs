fn foo(x: Box<isize>) -> isize {
    let y = &*x;
    free(x); // { dg-error ".E0505." "" { target *-*-* } }
    *y
}

fn free(_x: Box<isize>) {
}

fn main() {
}

