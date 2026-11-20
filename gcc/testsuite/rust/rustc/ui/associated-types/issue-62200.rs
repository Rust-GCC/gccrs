struct S {}

trait T<'a> {
    type A;
}

impl T<'_> for S {
    type A = u32;
}

fn foo(x: impl Fn(<S as T<'_>>::A) -> <S as T<'_>>::A) {}
// { dg-error ".E0582." "" { target *-*-* } .-1 }
// { dg-note ".E0582." "" { target *-*-* } .-2 }

fn main() {}

