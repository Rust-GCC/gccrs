pub fn main() {
    let x = "Hello " + "World!";
// { dg-error ".E0369." "" { target *-*-* } .-1 }

    // Make sure that the span outputs a warning
    // for not having an implementation for std::ops::Add
    // that won't output for the above string concatenation
    let y = World::Hello + World::Goodbye;
// { dg-error ".E0369." "" { target *-*-* } .-1 }

    let x = "Hello " + "World!".to_owned();
// { dg-error ".E0369." "" { target *-*-* } .-1 }
}

enum World {
    Hello,
    Goodbye,
}

fn foo() {
    let a = String::new();
    let b = String::new();
    let c = "";
    let d = "";
    let e = &a;
    let _ = &a + &b; // { dg-error ".E0369." "" { target *-*-* } }
    let _ = &a + b; // { dg-error ".E0369." "" { target *-*-* } }
    let _ = a + &b; // ok
    let _ = a + b; // { dg-error ".E0308." "" { target *-*-* } }
    let _ = e + b; // { dg-error ".E0369." "" { target *-*-* } }
    let _ = e + &b; // { dg-error ".E0369." "" { target *-*-* } }
    let _ = e + d; // { dg-error ".E0369." "" { target *-*-* } }
    let _ = e + &d; // { dg-error ".E0369." "" { target *-*-* } }
    let _ = &c + &d; // { dg-error ".E0369." "" { target *-*-* } }
    let _ = &c + d; // { dg-error ".E0369." "" { target *-*-* } }
    let _ = c + &d; // { dg-error ".E0369." "" { target *-*-* } }
    let _ = c + d; // { dg-error ".E0369." "" { target *-*-* } }
}

