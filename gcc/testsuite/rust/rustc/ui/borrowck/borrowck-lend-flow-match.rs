fn separate_arms() {
    // Here both arms perform assignments, but only one is illegal.

    let mut x = None;
    match x {
        None => {
            // It is ok to reassign x here, because there is in
            // fact no outstanding loan of x!
            x = Some(0);
        }
        Some(ref r) => {
            x = Some(1); // { dg-error ".E0506." "" { target *-*-* } }
            drop(r);
        }
    }
}

fn main() {}

