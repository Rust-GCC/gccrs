// Beginners write `mod.item` when they should write `mod::item`.
// This tests that we suggest the latter when we encounter the former.

pub mod a {
    pub const I: i32 = 1;

    pub fn f() -> i32 { 2 }

    pub mod b {
        pub const J: i32 = 3;

        pub fn g() -> i32 { 4 }
    }
}

fn h1() -> i32 {
    a.I
// { dg-error ".E0423." "" { target *-*-* } .-1 }
}

fn h2() -> i32 {
    a.g()
// { dg-error ".E0423." "" { target *-*-* } .-1 }
}

fn h3() -> i32 {
    a.b.J
// { dg-error ".E0423." "" { target *-*-* } .-1 }
}

fn h4() -> i32 {
    a::b.J
// { dg-error ".E0423." "" { target *-*-* } .-1 }
}

fn h5() {
    a.b.f();
// { dg-error ".E0423." "" { target *-*-* } .-1 }
    let v = Vec::new();
    v.push(a::b);
// { dg-error ".E0423." "" { target *-*-* } .-1 }
}

fn h6() -> i32 {
    a::b.f()
// { dg-error ".E0423." "" { target *-*-* } .-1 }
}

fn h7() {
    a::b
// { dg-error ".E0423." "" { target *-*-* } .-1 }
}

fn h8() -> i32 {
    a::b()
// { dg-error ".E0423." "" { target *-*-* } .-1 }
}

fn main() {}

