fn main() {
    let mut op = Some(2);
    match op {
        Some(ref v) => { let a = &mut v; },
// { dg-error ".E0596." "" { target *-*-* } .-1 }
        None => {},
    }
}

