fn main() {
    Vec::<[(); 1 + for x in 0..1 {}]>::new();
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }
}

