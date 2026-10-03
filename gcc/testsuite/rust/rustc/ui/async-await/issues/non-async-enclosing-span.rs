// edition:2018

async fn do_the_thing() -> u8 {
    8
}
// #63398: point at the enclosing scope and not the previously seen closure
fn main() {  // { dg-note "" "" { target *-*-* } }
    let x = move || {};
    let y = do_the_thing().await; // { dg-error ".E0728." "" { target *-*-* } }
// { dg-note ".E0728." "" { target *-*-* } .-1 }
}

