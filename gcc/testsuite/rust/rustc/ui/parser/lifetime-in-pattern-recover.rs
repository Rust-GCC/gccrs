fn main() {
    let &'a x = &0; // { dg-error "" "" { target *-*-* } }
    let &'a mut y = &mut 0; // { dg-error "" "" { target *-*-* } }

    let _recovery_witness: () = 0; // { dg-error ".E0308." "" { target *-*-* } }
}

