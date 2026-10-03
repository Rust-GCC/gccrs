// compile-flags: --edition 2018

#![feature(try_blocks)]

fn foo() -> Option<()> { Some(()) }

fn main() {
    let _: Option<f32> = try {
        foo()?;
        42
// { dg-error ".E0271." "" { target *-*-* } .-1 }
    };

    let _: Option<i32> = try {
        foo()?;
    };
// { dg-error ".E0271." "" { target *-*-* } .-1 }
}

