// ignore-tidy-linelength
// aux-build:unstable_generic_param.rs
#![feature(unstable_default6)]

extern crate unstable_generic_param;

use unstable_generic_param::*;

struct R;

impl Trait1 for S {
    fn foo() -> () { () } // ok
}

struct S;

impl Trait1<usize> for S { // { dg-error ".E0658." "" { target *-*-* } }
    fn foo() -> usize { 0 }
}

impl Trait1<isize> for S { // { dg-error ".E0658." "" { target *-*-* } }
    fn foo() -> isize { 0 }
}

impl Trait2<usize> for S { // { dg-error ".E0658." "" { target *-*-* } }
    fn foo() -> usize { 0 }
}

impl Trait3<usize> for S {
    fn foo() -> usize { 0 } // ok
}

fn main() {
    let _ = S;

    let _: Struct1<isize> = Struct1 { field: 1 }; // { dg-error ".E0658." "" { target *-*-* } }

    let _ = STRUCT1; // ok
    let _: Struct1 = STRUCT1; // ok
    let _: Struct1<usize> = STRUCT1; // { dg-error ".E0658." "" { target *-*-* } }
    let _: Struct1<isize> = Struct1 { field: 0 }; // { dg-error ".E0658." "" { target *-*-* } }

    // Instability is not enforced for generic type parameters used in public fields.
    // Note how the unstable type default `usize` leaks,
    // and can be used without the 'unstable_default' feature.
    let _ = STRUCT1.field;
    let _ = Struct1 { field: 1 };
    let _ = Struct1 { field: () };
    let _ = Struct1 { field: 1isize };
    let _: Struct1 = Struct1 { field: 1 };
    let _: usize = STRUCT1.field;
    let _ = STRUCT1.field + 1;
    let _ = STRUCT1.field + 1usize;

    let _ = Struct2 { field: 1 }; // ok
    let _: Struct2 = Struct2 { field: 1 }; // ok
    let _: Struct2<usize> = Struct2 { field: 1 }; // ok

    let _ = STRUCT2;
    let _: Struct2 = STRUCT2; // ok
    let _: Struct2<usize> = STRUCT2; // ok
    let _: Struct2<isize> = Struct2 { field: 0 }; // ok
    let _ = STRUCT2.field; // ok
    let _: usize = STRUCT2.field; // ok
    let _ = STRUCT2.field + 1; // ok
    let _ = STRUCT2.field + 1usize; // ok

    let _ = STRUCT3;
    let _: Struct3 = STRUCT3; // ok
    let _: Struct3<isize, usize> = STRUCT3; // { dg-error ".E0658." "" { target *-*-* } }
    let _: Struct3<isize> = STRUCT3; // ok
    let _: Struct3<isize, isize> = Struct3 { field1: 0, field2: 0 }; // { dg-error ".E0658." "" { target *-*-* } }
    let _: Struct3<usize, usize> = Struct3 { field1: 0, field2: 0 }; // { dg-error ".E0658." "" { target *-*-* } }
    let _ = STRUCT3.field1; // ok
    let _: isize = STRUCT3.field1; // ok
    let _ = STRUCT3.field1 + 1; // ok
    // Note the aforementioned leak.
    let _: usize = STRUCT3.field2; // ok
    let _: Struct3<usize> = Struct3 { field1: 0, field2: 0 }; // ok
    let _ = STRUCT3.field2 + 1; // ok
    let _ = STRUCT3.field2 + 1usize; // ok

    let _ = STRUCT4;
    let _: Struct4<isize> = Struct4 { field: 1 };
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
    let _ = STRUCT4;
    let _: Struct4 = STRUCT4; // { dg-error "" "" { target *-*-* } }
    let _: Struct4<usize> = STRUCT4; // { dg-error "" "" { target *-*-* } }
    let _: Struct4<isize> = Struct4 { field: 0 };
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }

    let _ = STRUCT5;
    let _: Struct5<isize> = Struct5 { field: 1 }; // { dg-error ".E0658." "" { target *-*-* } }
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }
// { dg-error ".E0658." "" { target *-*-* } .-3 }
    let _ = STRUCT5;
    let _: Struct5 = STRUCT5; // { dg-error "" "" { target *-*-* } }
    let _: Struct5<usize> = STRUCT5; // { dg-error ".E0658." "" { target *-*-* } }
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    let _: Struct5<isize> = Struct5 { field: 0 }; // { dg-error ".E0658." "" { target *-*-* } }
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }
// { dg-error ".E0658." "" { target *-*-* } .-3 }

    let _: Struct6<isize> = Struct6 { field: 1 }; // ok
    let _: Struct6<isize> = Struct6 { field: 0 }; // ok

    let _: Alias1<isize> = Alias1::Some(1); // { dg-error ".E0658." "" { target *-*-* } }

    let _ = ALIAS1; // ok
    let _: Alias1 = ALIAS1; // ok
    let _: Alias1<usize> = ALIAS1; // { dg-error ".E0658." "" { target *-*-* } }
    let _: Alias1<isize> = Alias1::Some(0); // { dg-error ".E0658." "" { target *-*-* } }

    // Instability is not enforced for generic type parameters used in public fields.
    // Note how the unstable type default `usize` leaks,
    // and can be used without the 'unstable_default' feature.
    let _ = Alias1::Some(1);
    let _ = Alias1::Some(());
    let _ = Alias1::Some(1isize);
    let _: Alias1 = Alias1::Some(1);
    let _: usize = ALIAS1.unwrap();
    let _ = ALIAS1.unwrap() + 1;
    let _ = ALIAS1.unwrap() + 1usize;

    let _ = Alias2::Some(1); // ok
    let _: Alias2 = Alias2::Some(1); // ok
    let _: Alias2<usize> = Alias2::Some(1); // ok

    let _ = ALIAS2;
    let _: Alias2 = ALIAS2; // ok
    let _: Alias2<usize> = ALIAS2; // ok
    let _: Alias2<isize> = Alias2::Some(0); // ok
    let _ = ALIAS2.unwrap(); // ok
    let _: usize = ALIAS2.unwrap(); // ok
    let _ = ALIAS2.unwrap() + 1; // ok
    let _ = ALIAS2.unwrap() + 1usize; // ok

    let _ = ALIAS3;
    let _: Alias3 = ALIAS3; // ok
    let _: Alias3<isize, usize> = ALIAS3; // { dg-error ".E0658." "" { target *-*-* } }
    let _: Alias3<isize> = ALIAS3; // ok
    let _: Alias3<isize, isize> = Alias3::Ok(0); // { dg-error ".E0658." "" { target *-*-* } }
    let _: Alias3<usize, usize> = Alias3::Ok(0); // { dg-error ".E0658." "" { target *-*-* } }
    let _ = ALIAS3.unwrap(); // ok
    let _: isize = ALIAS3.unwrap(); // ok
    let _ = ALIAS3.unwrap() + 1; // ok
    // Note the aforementioned leak.
    let _: usize = ALIAS3B.unwrap_err(); // ok
    let _: Alias3<usize> = Alias3::Err(0); // ok
    let _ = ALIAS3B.unwrap_err() + 1; // ok
    let _ = ALIAS3B.unwrap_err() + 1usize; // ok

    let _ = ALIAS4;
    let _: Alias4<isize> = Alias4::Some(1);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    let _ = ALIAS4;
    let _: Alias4 = ALIAS4; // { dg-error "" "" { target *-*-* } }
    let _: Alias4<usize> = ALIAS4; // { dg-error "" "" { target *-*-* } }
    let _: Alias4<isize> = Alias4::Some(0);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

    let _ = ALIAS5;
    let _: Alias5<isize> = Alias5::Some(1); // { dg-error ".E0658." "" { target *-*-* } }
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }
    let _ = ALIAS5;
    let _: Alias5 = ALIAS5; // { dg-error "" "" { target *-*-* } }
    let _: Alias5<usize> = ALIAS5; // { dg-error ".E0658." "" { target *-*-* } }
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    let _: Alias5<isize> = Alias5::Some(0); // { dg-error ".E0658." "" { target *-*-* } }
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }

    let _: Alias6<isize> = Alias6::Some(1); // ok
    let _: Alias6<isize> = Alias6::Some(0); // ok

    let _: Enum1<isize> = Enum1::Some(1); // { dg-error ".E0658." "" { target *-*-* } }

    let _ = ENUM1; // ok
    let _: Enum1 = ENUM1; // ok
    let _: Enum1<usize> = ENUM1; // { dg-error ".E0658." "" { target *-*-* } }
    let _: Enum1<isize> = Enum1::Some(0); // { dg-error ".E0658." "" { target *-*-* } }

    // Instability is not enforced for generic type parameters used in public fields.
    // Note how the unstable type default `usize` leaks,
    // and can be used without the 'unstable_default' feature.
    let _ = Enum1::Some(1);
    let _ = Enum1::Some(());
    let _ = Enum1::Some(1isize);
    let _: Enum1 = Enum1::Some(1);
    if let Enum1::Some(x) = ENUM1 {let _: usize = x;}
    if let Enum1::Some(x) = ENUM1 {let _ = x + 1;}
    if let Enum1::Some(x) = ENUM1 {let _ = x + 1usize;}

    let _ = Enum2::Some(1); // ok
    let _: Enum2 = Enum2::Some(1); // ok
    let _: Enum2<usize> = Enum2::Some(1); // ok

    let _ = ENUM2;
    let _: Enum2 = ENUM2; // ok
    let _: Enum2<usize> = ENUM2; // ok
    let _: Enum2<isize> = Enum2::Some(0); // ok
    if let Enum2::Some(x) = ENUM2 {let _ = x;} // ok
    if let Enum2::Some(x) = ENUM2 {let _: usize = x;} // ok
    if let Enum2::Some(x) = ENUM2 {let _ = x + 1;} // ok
    if let Enum2::Some(x) = ENUM2 {let _ = x + 1usize;} // ok

    let _ = ENUM3;
    let _: Enum3 = ENUM3; // ok
    let _: Enum3<isize, usize> = ENUM3; // { dg-error ".E0658." "" { target *-*-* } }
    let _: Enum3<isize> = ENUM3; // ok
    let _: Enum3<isize, isize> = Enum3::Ok(0); // { dg-error ".E0658." "" { target *-*-* } }
    let _: Enum3<usize, usize> = Enum3::Ok(0); // { dg-error ".E0658." "" { target *-*-* } }
    if let Enum3::Ok(x) = ENUM3 {let _ = x;} // ok
    if let Enum3::Ok(x) = ENUM3 {let _: isize = x;} // ok
    if let Enum3::Ok(x) = ENUM3 {let _ = x + 1;} // ok
    // Note the aforementioned leak.
    if let Enum3::Err(x) = ENUM3B {let _: usize = x;} // ok
    let _: Enum3<usize> = Enum3::Err(0); // ok
    if let Enum3::Err(x) = ENUM3B {let _ = x + 1;} // ok
    if let Enum3::Err(x) = ENUM3B {let _ = x + 1usize;} // ok

    let _ = ENUM4;
    let _: Enum4<isize> = Enum4::Some(1);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    let _ = ENUM4;
    let _: Enum4 = ENUM4; // { dg-error "" "" { target *-*-* } }
    let _: Enum4<usize> = ENUM4; // { dg-error "" "" { target *-*-* } }
    let _: Enum4<isize> = Enum4::Some(0);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

    let _ = ENUM5;
    let _: Enum5<isize> = Enum5::Some(1); // { dg-error ".E0658." "" { target *-*-* } }
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }
    let _ = ENUM5;
    let _: Enum5 = ENUM5; // { dg-error "" "" { target *-*-* } }
    let _: Enum5<usize> = ENUM5; // { dg-error ".E0658." "" { target *-*-* } }
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    let _: Enum5<isize> = Enum5::Some(0); // { dg-error ".E0658." "" { target *-*-* } }
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }

    let _: Enum6<isize> = Enum6::Some(1); // ok
    let _: Enum6<isize> = Enum6::Some(0); // ok

    let _: Box1<isize, System> = Box1::new(1); // { dg-error ".E0658." "" { target *-*-* } }
    let _: Box1<isize> = Box1::new(1); // ok

    let _: Box2<isize, System> = Box2::new(1); // ok
    let _: Box2<isize> = Box2::new(1); // ok

    let _: Box3<isize> = Box3::new(1); // ok
}

