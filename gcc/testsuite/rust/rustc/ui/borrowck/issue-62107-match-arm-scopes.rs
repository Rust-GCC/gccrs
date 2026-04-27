fn main() {
    let e: i32;
    match e {
// { dg-error ".E0381." "" { target *-*-* } .-1 }
        ref u if true => {}
        ref v if true => {
            let tx = 0;
            &tx;
        }
        _ => (),
    }
}

