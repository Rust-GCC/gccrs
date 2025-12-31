fn main() { // we don't complain about the return type being `{integer}`
    let t = (42, 42);
    t.0::<isize>; // { dg-error "" "" { target *-*-* } }
}

fn foo() -> usize { // we don't complain about the return type being unit
    let t = (42, 42);
    t.0::<isize>; // { dg-error "" "" { target *-*-* } }
    42;
}

