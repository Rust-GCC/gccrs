#![stable(feature = "core", since = "1.6.0")]
#![feature(staged_api)]
#![feature(const_precise_live_drops, const_fn)]

enum Either<T, S> {
    Left(T),
    Right(S),
}

impl<T> Either<T, T> {
    #[stable(feature = "rust1", since = "1.0.0")]
    #[rustc_const_stable(feature = "foo", since = "1.0.0")]
    pub const fn unwrap(self) -> T {
// { dg-error ".E0493." "" { target *-*-* } .-1 }
        match self {
            Self::Left(t) => t,
            Self::Right(t) => t,
        }
    }
}

fn main() {}

