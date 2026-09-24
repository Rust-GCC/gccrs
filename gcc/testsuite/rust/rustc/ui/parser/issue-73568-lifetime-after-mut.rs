#![crate_type="lib"]
fn x<'a>(x: &mut 'a i32){} // { dg-error "" "" { target *-*-* } }

macro_rules! mac {
    ($lt:lifetime) => {
        fn w<$lt>(w: &mut $lt i32) {}
// { dg-error "" "" { target *-*-* } .-1 }
    }
}

mac!('a);

// avoid false positives
fn y<'a>(y: &mut 'a + Send) {
// { dg-error ".E0224." "" { target *-*-* } .-1 }
// { dg-warning ".E0224." "" { target *-*-* } .-2 }
// { dg-error ".E0224." "" { target *-*-* } .-3 }
    let z = y as &mut 'a + Send;
// { dg-error ".E0423." "" { target *-*-* } .-1 }
// { dg-warning ".E0423." "" { target *-*-* } .-2 }
}

