enum Either<T, U> { Left(T), Right(U) }
struct S(Either<usize, usize>);

fn main() {
    match S(Either::Left(5)) {
        Either::Right(_) => {}
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }
        _ => {}
    }
}

