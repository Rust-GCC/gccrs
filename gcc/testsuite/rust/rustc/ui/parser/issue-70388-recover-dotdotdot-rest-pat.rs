struct Foo(i32);

fn main() {
    let Foo(...) = Foo(0); // { dg-error "" "" { target *-*-* } }
    let [_, ..., _] = [0, 1]; // { dg-error "" "" { target *-*-* } }
    let _recovery_witness: () = 0; // { dg-error ".E0308." "" { target *-*-* } }
}

