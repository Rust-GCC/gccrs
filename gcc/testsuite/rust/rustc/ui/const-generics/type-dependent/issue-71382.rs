// revisions: full min
#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

struct Test;

fn pass() -> u8 {
    42
}

impl Test {
    pub fn call_me(&self) -> u8 {
        self.test::<pass>()
    }

    fn test<const FN: fn() -> u8>(&self) -> u8 {
// { dg-error "" "" { target *-*-* } .-1 }
        FN()
    }
}

fn main() {
    let x = Test;
    assert_eq!(x.call_me(), 42);
}

