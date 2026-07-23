// check-pass
// compile-flags: --edition 2018
// run-rustfix

#![feature(try_blocks)]
#![warn(unused_parens, unused_braces)]

fn consume<T>(_: Result<T, T>) -> T { todo!() }

fn main() {
    consume((try {}));
// { dg-warning "" "" { target *-*-* } .-1 }

    consume({ try {} });
// { dg-warning "" "" { target *-*-* } .-1 }

    match (try {}) {
// { dg-warning "" "" { target *-*-* } .-1 }
        Ok(()) | Err(()) => (),
    }

    if let Err(()) = (try {}) {}
// { dg-warning "" "" { target *-*-* } .-1 }

    match (try {}) {
// { dg-warning "" "" { target *-*-* } .-1 }
        Ok(()) | Err(()) => (),
    }
}

