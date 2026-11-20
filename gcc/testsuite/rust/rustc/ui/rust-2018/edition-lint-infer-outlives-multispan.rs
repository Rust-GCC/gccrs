#![allow(unused)]
#![deny(explicit_outlives_requirements)]


// These examples should live in edition-lint-infer-outlives.rs, but are split
// into this separate file because they can't be `rustfix`'d (and thus, can't
// be part of a `run-rustfix` test file) until rust-lang-nursery/rustfix#141
// is solved

mod structs {
    use std::fmt::Debug;

    struct TeeOutlivesAyIsDebugBee<'a, 'b, T: 'a + Debug + 'b> {
// { dg-error "" "" { target *-*-* } .-1 }
        tee: &'a &'b T
    }

    struct TeeWhereOutlivesAyIsDebugBee<'a, 'b, T> where T: 'a + Debug + 'b {
// { dg-error "" "" { target *-*-* } .-1 }
        tee: &'a &'b T
    }

    struct TeeYooOutlivesAyIsDebugBee<'a, 'b, T, U: 'a + Debug + 'b> {
// { dg-error "" "" { target *-*-* } .-1 }
        tee: T,
        yoo: &'a &'b U
    }

    struct TeeOutlivesAyYooBeeIsDebug<'a, 'b, T: 'a, U: 'b + Debug> {
// { dg-error "" "" { target *-*-* } .-1 }
        tee: &'a T,
        yoo: &'b U
    }

    struct TeeOutlivesAyYooIsDebugBee<'a, 'b, T: 'a, U: Debug + 'b> {
// { dg-error "" "" { target *-*-* } .-1 }
        tee: &'a T,
        yoo: &'b U
    }

    struct TeeOutlivesAyYooWhereBee<'a, 'b, T: 'a, U> where U: 'b {
// { dg-error "" "" { target *-*-* } .-1 }
        tee: &'a T,
        yoo: &'b U
    }

    struct TeeYooWhereOutlivesAyIsDebugBee<'a, 'b, T, U> where U: 'a + Debug + 'b {
// { dg-error "" "" { target *-*-* } .-1 }
        tee: T,
        yoo: &'a &'b U
    }

    struct TeeOutlivesAyYooWhereBeeIsDebug<'a, 'b, T: 'a, U> where U: 'b + Debug {
// { dg-error "" "" { target *-*-* } .-1 }
        tee: &'a T,
        yoo: &'b U
    }

    struct TeeOutlivesAyYooWhereIsDebugBee<'a, 'b, T: 'a, U> where U: Debug + 'b {
// { dg-error "" "" { target *-*-* } .-1 }
        tee: &'a T,
        yoo: &'b U
    }

    struct TeeWhereOutlivesAyYooWhereBeeIsDebug<'a, 'b, T, U> where T: 'a, U: 'b + Debug {
// { dg-error "" "" { target *-*-* } .-1 }
        tee: &'a T,
        yoo: &'b U
    }

    struct TeeWhereOutlivesAyYooWhereIsDebugBee<'a, 'b, T, U> where T: 'a, U: Debug + 'b {
// { dg-error "" "" { target *-*-* } .-1 }
        tee: &'a T,
        yoo: &'b U
    }

    struct BeeOutlivesAyTeeBee<'a, 'b: 'a, T: 'b> {
// { dg-error "" "" { target *-*-* } .-1 }
        tee: &'a &'b T,
    }

    struct BeeOutlivesAyTeeAyBee<'a, 'b: 'a, T: 'a + 'b> {
// { dg-error "" "" { target *-*-* } .-1 }
        tee: &'a &'b T,
    }

    struct BeeOutlivesAyTeeOutlivesAyIsDebugBee<'a, 'b: 'a, T: 'a + Debug + 'b> {
// { dg-error "" "" { target *-*-* } .-1 }
        tee: &'a &'b T
    }

    struct BeeWhereAyTeeWhereOutlivesAyIsDebugBee<'a, 'b, T> where 'b: 'a, T: 'a + Debug + 'b {
// { dg-error "" "" { target *-*-* } .-1 }
        tee: &'a &'b T
    }

    struct BeeOutlivesAyTeeYooOutlivesAyIsDebugBee<'a, 'b: 'a, T, U: 'a + Debug + 'b> {
// { dg-error "" "" { target *-*-* } .-1 }
        tee: T,
        yoo: &'a &'b U
    }

    struct BeeWhereAyTeeYooWhereOutlivesAyIsDebugBee<'a, 'b, T, U>
        where U: 'a + Debug + 'b, 'b: 'a
// { dg-error "" "" { target *-*-* } .-1 }
    {
        tee: T,
        yoo: &'a &'b U
    }
}

mod tuple_structs {
    use std::fmt::Debug;

    struct TeeOutlivesAyIsDebugBee<'a, 'b, T: 'a + Debug + 'b>(&'a &'b T);
// { dg-error "" "" { target *-*-* } .-1 }

    struct TeeWhereOutlivesAyIsDebugBee<'a, 'b, T>(&'a &'b T) where T: 'a + Debug + 'b;
// { dg-error "" "" { target *-*-* } .-1 }

    struct TeeYooOutlivesAyIsDebugBee<'a, 'b, T, U: 'a + Debug + 'b>(T, &'a &'b U);
// { dg-error "" "" { target *-*-* } .-1 }

    struct TeeOutlivesAyYooBeeIsDebug<'a, 'b, T: 'a, U: 'b + Debug>(&'a T, &'b U);
// { dg-error "" "" { target *-*-* } .-1 }

    struct TeeOutlivesAyYooIsDebugBee<'a, 'b, T: 'a, U: Debug + 'b>(&'a T, &'b U);
// { dg-error "" "" { target *-*-* } .-1 }

    struct TeeOutlivesAyYooWhereBee<'a, 'b, T: 'a, U>(&'a T, &'b U) where U: 'b;
// { dg-error "" "" { target *-*-* } .-1 }

    struct TeeYooWhereOutlivesAyIsDebugBee<'a, 'b, T, U>(T, &'a &'b U) where U: 'a + Debug + 'b;
// { dg-error "" "" { target *-*-* } .-1 }

    struct TeeOutlivesAyYooWhereBeeIsDebug<'a, 'b, T: 'a, U>(&'a T, &'b U) where U: 'b + Debug;
// { dg-error "" "" { target *-*-* } .-1 }

    struct TeeOutlivesAyYooWhereIsDebugBee<'a, 'b, T: 'a, U>(&'a T, &'b U) where U: Debug + 'b;
// { dg-error "" "" { target *-*-* } .-1 }

    struct TeeWhereAyYooWhereBeeIsDebug<'a, 'b, T, U>(&'a T, &'b U) where T: 'a, U: 'b + Debug;
// { dg-error "" "" { target *-*-* } .-1 }

    struct TeeWhereAyYooWhereIsDebugBee<'a, 'b, T, U>(&'a T, &'b U) where T: 'a, U: Debug + 'b;
// { dg-error "" "" { target *-*-* } .-1 }

    struct BeeOutlivesAyTeeBee<'a, 'b: 'a, T: 'b>(&'a &'b T);
// { dg-error "" "" { target *-*-* } .-1 }

    struct BeeOutlivesAyTeeAyBee<'a, 'b: 'a, T: 'a + 'b>(&'a &'b T);
// { dg-error "" "" { target *-*-* } .-1 }

    struct BeeOutlivesAyTeeOutlivesAyIsDebugBee<'a, 'b: 'a, T: 'a + Debug + 'b>(&'a &'b T);
// { dg-error "" "" { target *-*-* } .-1 }

    struct BeeWhereAyTeeWhereAyIsDebugBee<'a, 'b, T>(&'a &'b T) where 'b: 'a, T: 'a + Debug + 'b;
// { dg-error "" "" { target *-*-* } .-1 }

    struct BeeOutlivesAyTeeYooOutlivesAyIsDebugBee<'a, 'b: 'a, T, U: 'a + Debug + 'b>(T, &'a &'b U);
// { dg-error "" "" { target *-*-* } .-1 }

    struct BeeWhereAyTeeYooWhereAyIsDebugBee<'a, 'b, T, U>(T, &'a &'b U)
        where U: 'a + Debug + 'b, 'b: 'a;
// { dg-error "" "" { target *-*-* } .-1 }
}

mod enums {
    use std::fmt::Debug;

    enum TeeOutlivesAyIsDebugBee<'a, 'b, T: 'a + Debug + 'b> {
// { dg-error "" "" { target *-*-* } .-1 }
        V { tee: &'a &'b T },
    }

    enum TeeWhereOutlivesAyIsDebugBee<'a, 'b, T> where T: 'a + Debug + 'b {
// { dg-error "" "" { target *-*-* } .-1 }
        V(&'a &'b T),
    }

    enum TeeYooOutlivesAyIsDebugBee<'a, 'b, T, U: 'a + Debug + 'b> {
// { dg-error "" "" { target *-*-* } .-1 }
        V { tee: T, },
        W(&'a &'b U),
    }

    enum TeeOutlivesAyYooBeeIsDebug<'a, 'b, T: 'a, U: 'b + Debug> {
// { dg-error "" "" { target *-*-* } .-1 }
        V { tee: &'a T, yoo: &'b U },
        W,
    }

    enum TeeOutlivesAyYooIsDebugBee<'a, 'b, T: 'a, U: Debug + 'b> {
// { dg-error "" "" { target *-*-* } .-1 }
        V(&'a T, &'b U),
        W,
    }

    enum TeeOutlivesAyYooWhereBee<'a, 'b, T: 'a, U> where U: 'b {
// { dg-error "" "" { target *-*-* } .-1 }
        V { tee: &'a T },
        W(&'b U),
    }

    enum TeeYooWhereOutlivesAyIsDebugBee<'a, 'b, T, U> where U: 'a + Debug + 'b {
// { dg-error "" "" { target *-*-* } .-1 }
        V { tee: T, yoo: &'a &'b U },
        W,
    }

    enum TeeOutlivesAyYooWhereBeeIsDebug<'a, 'b, T: 'a, U> where U: 'b + Debug {
// { dg-error "" "" { target *-*-* } .-1 }
        V(&'a T, &'b U),
        W,
    }

    enum TeeOutlivesAyYooWhereIsDebugBee<'a, 'b, T: 'a, U> where U: Debug + 'b {
// { dg-error "" "" { target *-*-* } .-1 }
        V { tee: &'a T },
        W(&'b U)
    }

    enum TeeWhereOutlivesAyYooWhereBeeIsDebug<'a, 'b, T, U> where T: 'a, U: 'b + Debug {
// { dg-error "" "" { target *-*-* } .-1 }
        V { tee: &'a T, yoo: &'b U },
        W,
    }

    enum TeeWhereOutlivesAyYooWhereIsDebugBee<'a, 'b, T, U> where T: 'a, U: Debug + 'b {
// { dg-error "" "" { target *-*-* } .-1 }
        V(&'a T, &'b U),
        W,
    }

    enum BeeOutlivesAyTeeBee<'a, 'b: 'a, T: 'b> {
// { dg-error "" "" { target *-*-* } .-1 }
        V { tee: &'a &'b T },
    }

    enum BeeOutlivesAyTeeAyBee<'a, 'b: 'a, T: 'a + 'b> {
// { dg-error "" "" { target *-*-* } .-1 }
        V { tee: &'a &'b T },
        W,
    }

    enum BeeOutlivesAyTeeOutlivesAyIsDebugBee<'a, 'b: 'a, T: 'a + Debug + 'b> {
// { dg-error "" "" { target *-*-* } .-1 }
        V { tee: &'a &'b T },
    }

    enum BeeWhereAyTeeWhereOutlivesAyIsDebugBee<'a, 'b, T> where 'b: 'a, T: 'a + Debug + 'b {
// { dg-error "" "" { target *-*-* } .-1 }
        V(&'a &'b T),
    }

    enum BeeOutlivesAyTeeYooOutlivesAyIsDebugBee<'a, 'b: 'a, T, U: 'a + Debug + 'b> {
// { dg-error "" "" { target *-*-* } .-1 }
        V { tee: T },
        W(&'a &'b U),
    }

    enum BeeWhereAyTeeYooWhereOutlivesAyIsDebugBee<'a, 'b, T, U> where U: 'a + Debug + 'b, 'b: 'a {
// { dg-error "" "" { target *-*-* } .-1 }
        V { tee: T, yoo: &'a &'b U },
    }
}

mod unions {
    use std::fmt::Debug;

    union TeeOutlivesAyIsDebugBee<'a, 'b, T: 'a + Debug + 'b> {
// { dg-error "" "" { target *-*-* } .-1 }
        tee: &'a &'b T
    }

    union TeeWhereOutlivesAyIsDebugBee<'a, 'b, T> where T: 'a + Debug + 'b {
// { dg-error "" "" { target *-*-* } .-1 }
        tee: &'a &'b T
    }

    union TeeYooOutlivesAyIsDebugBee<'a, 'b, T, U: 'a + Debug + 'b> {
// { dg-error "" "" { target *-*-* } .-1 }
        tee: *const T,
        yoo: &'a &'b U
    }

    union TeeOutlivesAyYooBeeIsDebug<'a, 'b, T: 'a, U: 'b + Debug> {
// { dg-error "" "" { target *-*-* } .-1 }
        tee: &'a T,
        yoo: &'b U
    }

    union TeeOutlivesAyYooIsDebugBee<'a, 'b, T: 'a, U: Debug + 'b> {
// { dg-error "" "" { target *-*-* } .-1 }
        tee: &'a T,
        yoo: &'b U
    }

    union TeeOutlivesAyYooWhereBee<'a, 'b, T: 'a, U> where U: 'b {
// { dg-error "" "" { target *-*-* } .-1 }
        tee: &'a T,
        yoo: &'b U
    }

    union TeeYooWhereOutlivesAyIsDebugBee<'a, 'b, T, U> where U: 'a + Debug + 'b {
// { dg-error "" "" { target *-*-* } .-1 }
        tee: *const T,
        yoo: &'a &'b U
    }

    union TeeOutlivesAyYooWhereBeeIsDebug<'a, 'b, T: 'a, U> where U: 'b + Debug {
// { dg-error "" "" { target *-*-* } .-1 }
        tee: &'a T,
        yoo: &'b U
    }

    union TeeOutlivesAyYooWhereIsDebugBee<'a, 'b, T: 'a, U> where U: Debug + 'b {
// { dg-error "" "" { target *-*-* } .-1 }
        tee: &'a T,
        yoo: &'b U
    }

    union TeeWhereOutlivesAyYooWhereBeeIsDebug<'a, 'b, T, U> where T: 'a, U: 'b + Debug {
// { dg-error "" "" { target *-*-* } .-1 }
        tee: &'a T,
        yoo: &'b U
    }

    union TeeWhereOutlivesAyYooWhereIsDebugBee<'a, 'b, T, U> where T: 'a, U: Debug + 'b {
// { dg-error "" "" { target *-*-* } .-1 }
        tee: &'a T,
        yoo: &'b U
    }

    union BeeOutlivesAyTeeBee<'a, 'b: 'a, T: 'b> {
// { dg-error "" "" { target *-*-* } .-1 }
        tee: &'a &'b T,
    }

    union BeeOutlivesAyTeeAyBee<'a, 'b: 'a, T: 'a + 'b> {
// { dg-error "" "" { target *-*-* } .-1 }
        tee: &'a &'b T,
    }

    union BeeOutlivesAyTeeOutlivesAyIsDebugBee<'a, 'b: 'a, T: 'a + Debug + 'b> {
// { dg-error "" "" { target *-*-* } .-1 }
        tee: &'a &'b T
    }

    union BeeWhereAyTeeWhereOutlivesAyIsDebugBee<'a, 'b, T> where 'b: 'a, T: 'a + Debug + 'b {
// { dg-error "" "" { target *-*-* } .-1 }
        tee: &'a &'b T
    }

    union BeeOutlivesAyTeeYooOutlivesAyIsDebugBee<'a, 'b: 'a, T, U: 'a + Debug + 'b> {
// { dg-error "" "" { target *-*-* } .-1 }
        tee: *const T,
        yoo: &'a &'b U
    }

    union BeeWhereAyTeeYooWhereOutlivesAyIsDebugBee<'a, 'b, T, U> where U: 'a + Debug + 'b, 'b: 'a {
// { dg-error "" "" { target *-*-* } .-1 }
        tee: *const T,
        yoo: &'a &'b U
    }
}

fn main() {}

