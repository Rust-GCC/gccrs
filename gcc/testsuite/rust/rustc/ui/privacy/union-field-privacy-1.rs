mod m {
    pub union U {
        pub a: u8,
        pub(super) b: u8,
        c: u8,
    }
}

fn main() { unsafe {
    let u = m::U { a: 0 }; // OK
    let u = m::U { b: 0 }; // OK
    let u = m::U { c: 0 }; // { dg-error ".E0451." "" { target *-*-* } }

    let m::U { a } = u; // OK
    let m::U { b } = u; // OK
    let m::U { c } = u; // { dg-error ".E0451." "" { target *-*-* } }
}}

