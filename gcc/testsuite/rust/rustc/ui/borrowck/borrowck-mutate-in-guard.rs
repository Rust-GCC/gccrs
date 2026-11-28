enum Enum<'a> {
    A(&'a isize),
    B(bool),
}

fn foo() -> isize {
    let mut n = 42;
    let mut x = Enum::A(&mut n);
    match x {
        Enum::A(_) if { x = Enum::B(false); false } => 1,
// { dg-error ".E0510." "" { target *-*-* } .-1 }
        Enum::A(_) if { let y = &mut x; *y = Enum::B(false); false } => 1,
// { dg-error ".E0510." "" { target *-*-* } .-1 }
        Enum::A(p) => *p,
        Enum::B(_) => 2,
    }
}

fn main() {
    foo();
}

