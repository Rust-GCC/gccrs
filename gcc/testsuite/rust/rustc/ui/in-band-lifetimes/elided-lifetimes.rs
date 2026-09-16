// run-rustfix
// edition:2018

#![allow(unused)]
#![deny(elided_lifetimes_in_paths)]
// { dg-note "" "" { target *-*-* } .-1 }

use std::cell::{RefCell, Ref};


struct Foo<'a> { x: &'a u32 }

fn foo(x: &Foo) {
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
}

fn bar(x: &Foo<'_>) {}


struct Wrapped<'a>(&'a str);

struct WrappedWithBow<'a> {
    gift: &'a str
}

struct MatchedSet<'a, 'b> {
    one: &'a str,
    another: &'b str,
}

fn wrap_gift(gift: &str) -> Wrapped {
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
    Wrapped(gift)
}

fn wrap_gift_with_bow(gift: &str) -> WrappedWithBow {
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
    WrappedWithBow { gift }
}

fn inspect_matched_set(set: MatchedSet) {
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
    println!("{} {}", set.one, set.another);
}

macro_rules! autowrapper {
    ($type_name:ident, $fn_name:ident, $lt:lifetime) => {
        struct $type_name<$lt> {
            gift: &$lt str
        }

        fn $fn_name(gift: &str) -> $type_name {
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
            $type_name { gift }
        }
    }
}

autowrapper!(Autowrapped, autowrap_gift, 'a);
// { dg-note "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }

macro_rules! anytuple_ref_ty {
    ($($types:ty),*) => {
        Ref<($($types),*)>
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
    }
}

fn main() {
    let honesty = RefCell::new((4, 'e'));
    let loyalty: Ref<(u32, char)> = honesty.borrow();
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
    let generosity = Ref::map(loyalty, |t| &t.0);

    let laughter = RefCell::new((true, "magic"));
    let yellow: anytuple_ref_ty!(bool, &str) = laughter.borrow();
// { dg-note "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }
}

