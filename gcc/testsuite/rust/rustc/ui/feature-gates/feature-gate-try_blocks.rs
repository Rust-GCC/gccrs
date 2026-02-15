// compile-flags: --edition 2018

pub fn main() {
    let try_result: Option<_> = try { // { dg-error ".E0658." "" { target *-*-* } }
        let x = 5;
        x
    };
    assert_eq!(try_result, Some(5));
}

