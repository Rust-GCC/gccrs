macro_rules! empty {
    () => { }
}

fn foo() -> bool { // { dg-error ".E0308." "" { target *-*-* } }
    { true } // { dg-error ".E0308." "" { target *-*-* } }
    empty!();
}

fn main() {}

