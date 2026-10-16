fn main() {
    struct Thing(u8, [u8; 0]);
    let foo = core::iter::empty();

    for Thing(x[]) in foo {}
// { dg-error "" "" { target *-*-* } .-1 }
}

const RECOVERY_WITNESS: () = 0; // { dg-error ".E0308." "" { target *-*-* } }

