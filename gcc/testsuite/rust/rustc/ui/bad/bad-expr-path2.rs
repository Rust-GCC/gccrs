mod m1 {
    pub mod arguments {}
}

fn main(arguments: Vec<String>) { // { dg-error ".E0580." "" { target *-*-* } }
    log(debug, m1::arguments);
// { dg-error ".E0423." "" { target *-*-* } .-1 }
// { dg-error ".E0423." "" { target *-*-* } .-2 }
// { dg-error ".E0423." "" { target *-*-* } .-3 }
}

