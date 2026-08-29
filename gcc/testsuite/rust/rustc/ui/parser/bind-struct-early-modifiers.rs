fn main() {
    struct Foo { x: isize }
    match (Foo { x: 10 }) {
        Foo { ref x: ref x } => {}, // { dg-error "" "" { target *-*-* } }
        _ => {}
    }
}

