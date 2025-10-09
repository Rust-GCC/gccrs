const F: &'static dyn PartialEq<u32> = &7u32;

fn main() {
    let a: &dyn PartialEq<u32> = &7u32;
    match a {
        F => panic!(), // { dg-error "" "" { target *-*-* } }
    }
}

