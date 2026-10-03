fn main() {
    assert!();  // { dg-error "" "" { target *-*-* } }
    assert!(struct); // { dg-error "" "" { target *-*-* } }
    debug_assert!(); // { dg-error "" "" { target *-*-* } }
    debug_assert!(struct); // { dg-error "" "" { target *-*-* } }
}

