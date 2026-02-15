struct X(i32,i32,i32);

fn main() {
    let a = X(1, 2, 3);
    let b = a.1suffix;
// { dg-error "" "" { target *-*-* } .-1 }
    println!("{}", b);
    let c = (1, 2, 3);
    let d = c.1suffix;
// { dg-error "" "" { target *-*-* } .-1 }
    println!("{}", d);
    let s = X { 0suffix: 0, 1: 1, 2: 2 };
// { dg-error "" "" { target *-*-* } .-1 }
    match s {
        X { 0suffix: _, .. } => {}
// { dg-error "" "" { target *-*-* } .-1 }
    }
}

