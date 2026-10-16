// Test that even unboxed closures that are capable of mutating their
// environment cannot mutate captured variables that have not been
// declared mutable (#18335)

fn set(x: &mut usize) { *x = 0; }

fn main() {
    let x = 0;
    move || x = 1; // { dg-error ".E0594." "" { target *-*-* } }
    move || set(&mut x); // { dg-error ".E0596." "" { target *-*-* } }
    move || x = 1; // { dg-error ".E0594." "" { target *-*-* } }
    move || set(&mut x); // { dg-error ".E0596." "" { target *-*-* } }
    || x = 1; // { dg-error ".E0594." "" { target *-*-* } }
    || set(&mut x); // { dg-error ".E0596." "" { target *-*-* } }
    || x = 1; // { dg-error ".E0594." "" { target *-*-* } }
    || set(&mut x); // { dg-error ".E0596." "" { target *-*-* } }
}

