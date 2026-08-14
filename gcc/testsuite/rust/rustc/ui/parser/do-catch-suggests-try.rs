#![feature(try_blocks)]

fn main() {
    let _: Option<()> = do catch {};
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }

    let _recovery_witness: () = 1; // { dg-error ".E0308." "" { target *-*-* } }
}

