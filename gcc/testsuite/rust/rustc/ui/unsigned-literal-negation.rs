fn main() {
    let x = -1 as usize; // { dg-error ".E0600." "" { target *-*-* } }
    let x = (-1) as usize; // { dg-error ".E0600." "" { target *-*-* } }
    let x: u32 = -1; // { dg-error ".E0600." "" { target *-*-* } }
}

