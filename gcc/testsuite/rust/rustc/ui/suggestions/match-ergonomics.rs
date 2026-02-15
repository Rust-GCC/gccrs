fn main() {
    let x = vec![1i32];
    match &x[..] {
        [&v] => {}, // { dg-error ".E0308." "" { target *-*-* } }
        _ => {},
    }
    match x {
        [&v] => {}, // { dg-error ".E0529." "" { target *-*-* } }
        _ => {},
    }
    match &x[..] {
        [v] => {},
        _ => {},
    }
    match &x[..] {
        &[v] => {},
        _ => {},
    }
    match x {
        [v] => {}, // { dg-error ".E0529." "" { target *-*-* } }
        _ => {},
    }
    let y = 1i32;
    match &y {
        &v => {},
        _ => {},
    }
    match y {
        &v => {}, // { dg-error ".E0308." "" { target *-*-* } }
        _ => {},
    }
    match &y {
        v => {},
        _ => {},
    }
    match y {
        v => {},
        _ => {},
    }
    if let [&v] = &x[..] {} // { dg-error ".E0308." "" { target *-*-* } }
}

