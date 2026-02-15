enum A { B, C }

mod foo { pub fn bar() {} }

fn main() {
    match (true, false) {
        A::B => (),
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }
        _ => ()
    }
}

