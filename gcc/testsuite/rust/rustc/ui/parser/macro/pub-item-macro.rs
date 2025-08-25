// Issue #14660

macro_rules! priv_x {
    () => {
        static x: u32 = 0;
    };
}

macro_rules! pub_x { () => {
    pub priv_x!(); // { dg-error "" "" { target *-*-* } }
// { help "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
}}

mod foo {
    pub_x!();
}

fn main() {
    let y: u32 = foo::x; // { dg-error ".E0603." "" { target *-*-* } }
}

