macro_rules! foo {
    ($e:expr) => {}
}

foo!(1 + @); // { dg-error "" "" { target *-*-* } }
foo!(1 + @); // { dg-error "" "" { target *-*-* } }

fn main() {
    let _recovery_witness: () = 0; // { dg-error ".E0308." "" { target *-*-* } }
}

