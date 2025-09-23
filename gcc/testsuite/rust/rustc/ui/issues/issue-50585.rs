fn main() {
    |y: Vec<[(); for x in 0..2 {}]>| {};
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
}

