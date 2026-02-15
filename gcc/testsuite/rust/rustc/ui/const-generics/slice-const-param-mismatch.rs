// revisions: full min

#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]


struct ConstString<const T: &'static str>;
// { dg-error "" "" { target *-*-* } .-1 }
struct ConstBytes<const T: &'static [u8]>;
// { dg-error "" "" { target *-*-* } .-1 }

pub fn main() {
    let _: ConstString<"Hello"> = ConstString::<"Hello">;
    let _: ConstString<"Hello"> = ConstString::<"World">; // { dg-error "" "" { target *-*-* } }
    let _: ConstString<"ℇ㇈↦"> = ConstString::<"ℇ㇈↦">;
    let _: ConstString<"ℇ㇈↦"> = ConstString::<"ℇ㇈↥">; // { dg-error "" "" { target *-*-* } }
    let _: ConstBytes<b"AAA"> = ConstBytes::<{&[0x41, 0x41, 0x41]}>;
    let _: ConstBytes<b"AAA"> = ConstBytes::<b"BBB">; // { dg-error "" "" { target *-*-* } }
}

