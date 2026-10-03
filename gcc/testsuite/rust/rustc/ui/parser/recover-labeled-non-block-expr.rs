fn main() {
    'label: 1 + 1; // { dg-error "" "" { target *-*-* } }

    let _recovery_witness: () = 0; // { dg-error ".E0308." "" { target *-*-* } }
}

