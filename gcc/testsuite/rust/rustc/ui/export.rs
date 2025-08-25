mod foo {
    pub fn x(y: isize) { log(debug, y); }
// { dg-error ".E0425." "" { target *-*-* } .-1 }
// { dg-error ".E0425." "" { target *-*-* } .-2 }
    fn z(y: isize) { log(debug, y); }
// { dg-error ".E0425." "" { target *-*-* } .-1 }
// { dg-error ".E0425." "" { target *-*-* } .-2 }
}

fn main() { foo::z(10); } // { dg-error ".E0603." "" { target *-*-* } }

