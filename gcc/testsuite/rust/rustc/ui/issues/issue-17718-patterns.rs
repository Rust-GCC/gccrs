static A1: usize = 1;
static mut A2: usize = 1;
const A3: usize = 1;

fn main() {
    match 1 {
        A1 => {} // { dg-error ".E0530." "" { target *-*-* } }
        A2 => {} // { dg-error ".E0530." "" { target *-*-* } }
        A3 => {}
        _ => {}
    }
}

