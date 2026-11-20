const C: u8 = 0;

fn main() {
    match 1u8 {
        mut C => {} // { dg-error ".E0530." "" { target *-*-* } }
        _ => {}
    }
}

