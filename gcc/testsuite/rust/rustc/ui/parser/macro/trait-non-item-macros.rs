macro_rules! bah {
    ($a:expr) => {
        $a
// { dg-error "" "" { target *-*-* } .-1 }
}

trait Bar {
    bah!(2);
}

fn main() {
    let _recovery_witness: () = 0; // { dg-error ".E0308." "" { target *-*-* } }
}

