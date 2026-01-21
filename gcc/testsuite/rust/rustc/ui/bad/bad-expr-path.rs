mod m1 {}

fn main(arguments: Vec<String>) { // { dg-error ".E0580." "" { target *-*-* } }
    log(debug, m1::arguments);
// { dg-error ".E0425." "" { target *-*-* } .-1 }
// { dg-error ".E0425." "" { target *-*-* } .-2 }
// { dg-error ".E0425." "" { target *-*-* } .-3 }
}

