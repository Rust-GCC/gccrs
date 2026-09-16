fn main() {
    struct Test(&'static u8, [u8; 0]);
    let x = Test(&0, []);

    let Test(&desc[..]) = x;
// { dg-error "" "" { target *-*-* } .-1 }
}

const RECOVERY_WITNESS: () = 0; // { dg-error ".E0308." "" { target *-*-* } }

