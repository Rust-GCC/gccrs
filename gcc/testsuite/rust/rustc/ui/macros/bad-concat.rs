fn main() {
    let x: u32 = 42;
    let y: f64 = 3.14;
    let z = "foo";
    let _ = concat!(x, y, z, "bar");
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }
}

