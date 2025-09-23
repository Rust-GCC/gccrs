union U {
    a: usize,
    b: usize,
}

const C: U = U { a: 10 };

fn main() {
    match C {
        C => {} // { dg-error "" "" { target *-*-* } }
        _ => {}
    }
}

