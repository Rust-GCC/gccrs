fn main() {
    println!(3 + 4);
// { dg-error "" "" { target *-*-* } .-1 }
    println!(3, 4);
// { dg-error "" "" { target *-*-* } .-1 }
}

