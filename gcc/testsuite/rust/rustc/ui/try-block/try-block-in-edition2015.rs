// compile-flags: --edition 2015

pub fn main() {
    let try_result: Option<_> = try {
// { dg-error ".E0574." "" { target *-*-* } .-1 }
        let x = 5; // { dg-error "" "" { target *-*-* } }
        x
    };
    assert_eq!(try_result, Some(5));
}

