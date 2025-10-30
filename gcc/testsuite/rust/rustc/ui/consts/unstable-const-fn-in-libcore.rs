// This is a non-regression test for const-qualification of unstable items in libcore
// as explained in issue #67053.
// const-qualification could miss some `const fn`s if they were unstable and the feature
// gate was not enabled in libcore.

#![stable(feature = "core", since = "1.6.0")]
#![feature(rustc_const_unstable)]
#![feature(staged_api)]
#![feature(const_fn)]

enum Opt<T> {
    Some(T),
    None,
}

impl<T> Opt<T> {
    #[rustc_const_unstable(feature = "foo", issue = "none")]
    #[stable(feature = "rust1", since = "1.0.0")]
    const fn unwrap_or_else<F: FnOnce() -> T>(self, f: F) -> T {
// { dg-error ".E0493." "" { target *-*-* } .-1 }
// { dg-error ".E0493." "" { target *-*-* } .-2 }
        match self {
            Opt::Some(t) => t,
            Opt::None => f(), // { dg-error ".E0015." "" { target *-*-* } }
        }
    }
}

fn main() {}

