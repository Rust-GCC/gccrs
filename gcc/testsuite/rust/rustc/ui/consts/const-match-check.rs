// revisions: matchck eval1 eval2

#[cfg(matchck)]
const X: i32 = { let 0 = 0; 0 };
// { dg-error "" "" { target *-*-* } .-1 }

#[cfg(matchck)]
static Y: i32 = { let 0 = 0; 0 };
// { dg-error "" "" { target *-*-* } .-1 }

#[cfg(matchck)]
trait Bar {
    const X: i32 = { let 0 = 0; 0 };
// { dg-error "" "" { target *-*-* } .-1 }
}

#[cfg(matchck)]
impl Bar for () {
    const X: i32 = { let 0 = 0; 0 };
// { dg-error "" "" { target *-*-* } .-1 }
}

#[cfg(eval1)]
enum Foo {
    A = { let 0 = 0; 0 },
// { dg-error "" "" { target *-*-* } .-1 }
}

fn main() {
    #[cfg(eval2)]
    let x: [i32; { let 0 = 0; 0 }] = [];
// { dg-error "" "" { target *-*-* } .-1 }
}

