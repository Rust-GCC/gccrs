// compile-flags: -Z parse-only

#![feature(const_trait_bound_opt_out)]
#![allow(incomplete_features)]

struct S<T: ?const ?const Tr>;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

