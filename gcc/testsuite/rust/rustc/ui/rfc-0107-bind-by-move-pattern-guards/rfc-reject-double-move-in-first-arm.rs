struct A { a: Box<i32> }

fn foo(n: i32) {
    let x = A { a: Box::new(n) };
    let _y = match x {
        A { a: v } if { drop(v); true } => v,
// { dg-error ".E0507." "" { target *-*-* } .-1 }
        _ => Box::new(0),
    };
}

fn main() {
    foo(107);
}

