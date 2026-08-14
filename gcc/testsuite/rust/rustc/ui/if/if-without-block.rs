fn main() {
    let n = 1;
    if 5 == {
// { dg-note "" "" { target *-*-* } .-1 }
        println!("five");
    }
}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }

