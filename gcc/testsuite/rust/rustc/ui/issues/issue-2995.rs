fn bad (p: *const isize) {
    let _q: &isize = p as &isize; // { dg-error ".E0605." "" { target *-*-* } }
}

fn main() { }

