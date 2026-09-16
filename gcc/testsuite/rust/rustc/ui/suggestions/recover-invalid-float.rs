// run-rustfix

fn main() {
    let _: f32 = .3;
// { dg-error "" "" { target *-*-* } .-1 }
    let _: f32 = .42f32;
// { dg-error "" "" { target *-*-* } .-1 }
    let _: f64 = .5f64;
// { dg-error "" "" { target *-*-* } .-1 }
}

