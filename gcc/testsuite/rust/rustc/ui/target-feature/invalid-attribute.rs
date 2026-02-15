// ignore-arm
// ignore-aarch64
// ignore-wasm
// ignore-emscripten
// ignore-mips
// ignore-mips64
// ignore-powerpc
// ignore-powerpc64
// ignore-powerpc64le
// ignore-riscv64
// ignore-s390x
// ignore-sparc
// ignore-sparc64

#![feature(target_feature)]
#![warn(unused_attributes)]

#[target_feature = "+sse2"]
// { dg-error "" "" { target *-*-* } .-1 }
#[target_feature(enable = "foo")]
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }
#[target_feature(bar)]
// { dg-error "" "" { target *-*-* } .-1 }
#[target_feature(disable = "baz")]
// { dg-error "" "" { target *-*-* } .-1 }
unsafe fn foo() {}

#[target_feature(enable = "sse2")]
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-note ".E0658." "" { target *-*-* } .-2 }
fn bar() {}
// { dg-note "" "" { target *-*-* } .-1 }

#[target_feature(enable = "sse2")]
// { dg-error "" "" { target *-*-* } .-1 }
mod another {}
// { dg-note "" "" { target *-*-* } .-1 }

#[target_feature(enable = "sse2")]
// { dg-error "" "" { target *-*-* } .-1 }
const FOO: usize = 7;
// { dg-note "" "" { target *-*-* } .-1 }

#[target_feature(enable = "sse2")]
// { dg-error "" "" { target *-*-* } .-1 }
struct Foo;
// { dg-note "" "" { target *-*-* } .-1 }

#[target_feature(enable = "sse2")]
// { dg-error "" "" { target *-*-* } .-1 }
enum Bar {}
// { dg-note "" "" { target *-*-* } .-1 }

#[target_feature(enable = "sse2")]
// { dg-error "" "" { target *-*-* } .-1 }
union Qux {
// { dg-note "" "" { target *-*-* } .-1 }
    f1: u16,
    f2: u16,
}

#[target_feature(enable = "sse2")]
// { dg-error "" "" { target *-*-* } .-1 }
trait Baz {}
// { dg-note "" "" { target *-*-* } .-1 }

#[inline(always)]
// { dg-error "" "" { target *-*-* } .-1 }
#[target_feature(enable = "sse2")]
unsafe fn test() {}

trait Quux {
    fn foo();
}

impl Quux for Foo {
    #[target_feature(enable = "sse2")]
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-note ".E0658." "" { target *-*-* } .-2 }
    fn foo() {}
// { dg-note "" "" { target *-*-* } .-1 }
}

fn main() {
    #[target_feature(enable = "sse2")]
// { dg-error "" "" { target *-*-* } .-1 }
    unsafe {
        foo();
        bar();
    }
// { dg-note "" "" { target *-*-* } .-4 }

    #[target_feature(enable = "sse2")]
// { dg-error "" "" { target *-*-* } .-1 }
    || {};
// { dg-note "" "" { target *-*-* } .-1 }
}

