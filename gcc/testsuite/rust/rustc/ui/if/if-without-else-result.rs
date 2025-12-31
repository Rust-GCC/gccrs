fn main() {
    let a = if true { true };
// { dg-error ".E0317." "" { target *-*-* } .-1 }
// { dg-error ".E0317." "" { target *-*-* } .-2 }
    println!("{}", a);
}

