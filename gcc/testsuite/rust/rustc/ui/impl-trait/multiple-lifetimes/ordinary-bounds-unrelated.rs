// edition:2018

#![feature(member_constraints)]

trait Trait<'a, 'b> {}
impl<T> Trait<'_, '_> for T {}

// `Ordinary<'a> <: Ordinary<'b>` if `'a: 'b`, as with most types.
//
// I am purposefully avoiding the terms co- and contra-variant because
// their application to regions depends on how you interpreted Rust
// regions. -nikomatsakis
struct Ordinary<'a>(&'a u8);

// Here we get an error because none of our choices (either `'d` nor `'e`) are outlived
// by both `'a` and `'b`.

fn upper_bounds<'a, 'b, 'c, 'd, 'e>(a: Ordinary<'a>, b: Ordinary<'b>) -> impl Trait<'d, 'e>
// { dg-error ".E0700." "" { target *-*-* } .-1 }
where
    'a: 'e,
    'b: 'd,
{
    // Hidden type `Ordinary<'0>` with constraints:
    //
    // ```
    // 'a: '0
    // 'b: '0
    // 'a in ['d, 'e]
    // ```
    if condition() { a } else { b }
}

fn condition() -> bool {
    true
}

fn main() {}

