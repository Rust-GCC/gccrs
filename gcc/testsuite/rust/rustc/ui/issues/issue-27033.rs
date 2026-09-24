fn main() {
    match Some(1) {
        None @ _ => {} // { dg-error ".E0530." "" { target *-*-* } }
    };
    const C: u8 = 1;
    match 1 {
        C @ 2 => { // { dg-error ".E0530." "" { target *-*-* } }
            println!("{}", C);
        }
        _ => {}
    };
}

