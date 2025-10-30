// rust-lang/rust#52059: Regardless of whether you are moving out of a
// Drop type or just introducing an inadvertent alias via a borrow of
// one of its fields, it is useful to be reminded of the significance
// of the fact that the type implements Drop.

pub struct S<'a> { url: &'a mut String }

impl<'a> Drop for S<'a> { fn drop(&mut self) { } }

fn finish_1(s: S) -> &mut String {
    s.url
}
// { dg-error ".E0713." "" { target *-*-* } .-2 }

fn finish_2(s: S) -> &mut String {
    let p = &mut *s.url; p
}
// { dg-error ".E0713." "" { target *-*-* } .-2 }

fn finish_3(s: S) -> &mut String {
    let p: &mut _ = s.url; p
}
// { dg-error ".E0713." "" { target *-*-* } .-2 }

fn finish_4(s: S) -> &mut String {
    let p = s.url; p
}
// { dg-error ".E0509." "" { target *-*-* } .-2 }

fn main() {}

