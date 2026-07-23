#![feature(box_syntax)]

fn test(_x: &mut String) {}
fn test2(_x: &mut i32) {}

fn main() {
    let x: usize = String::new();
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    let x: &str = String::new();
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    let y = String::new();
    test(&y);
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    test2(&y);
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    let f;
    f = box f;
// { dg-error ".E0308." "" { target *-*-* } .-1 }

    let s = &mut String::new();
    s = format!("foo");
// { dg-error ".E0308." "" { target *-*-* } .-1 }
}

