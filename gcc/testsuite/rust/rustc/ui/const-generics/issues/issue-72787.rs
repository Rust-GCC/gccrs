// revisions: full min
#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

pub struct IsLessOrEqual<const LHS: u32, const RHS: u32>;
pub struct Condition<const CONDITION: bool>;
pub trait True {}

impl<const LHS: u32, const RHS: u32> True for IsLessOrEqual<LHS, RHS> where
    Condition<{ LHS <= RHS }>: True
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-2 }
{
}
impl True for Condition<true> {}

struct S<const I: u32, const J: u32>;
impl<const I: u32, const J: u32> S<I, J>
where
    IsLessOrEqual<I, 8>: True,
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    IsLessOrEqual<J, 8>: True,
    IsLessOrEqual<{ 8 - I }, { 8 - J }>: True,
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
// { dg-error "" "" { target *-*-* } .-4 }
// { dg-error "" "" { target *-*-* } .-5 }
// { dg-error "" "" { target *-*-* } .-2 }
    // Condition<{ 8 - I <= 8 - J }>: True,
{
    fn print() {
        println!("I {} J {}", I, J);
    }
}

fn main() {}

