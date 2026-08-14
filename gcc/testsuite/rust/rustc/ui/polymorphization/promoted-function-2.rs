// build-fail
// compile-flags:-Zpolymorphize=on
#![crate_type = "lib"]
#![feature(lazy_normalization_consts, rustc_attrs)]
// { dg-warning "" "" { target *-*-* } .-1 }

#[rustc_polymorphize_error]
fn test<T>() {
// { dg-error "" "" { target *-*-* } .-1 }
    let x = [0; 3 + 4];
}

pub fn caller() {
    test::<String>();
    test::<Vec<String>>();
}

