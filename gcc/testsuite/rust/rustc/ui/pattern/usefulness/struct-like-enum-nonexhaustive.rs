enum A {
    B { x: Option<isize> },
    C
}

fn main() {
    let x = A::B { x: Some(3) };
    match x {   // { dg-error ".E0004." "" { target *-*-* } }
        A::C => {}
        A::B { x: None } => {}
    }
}

