// Check that unresolved imports do not create additional errors and ICEs

mod m {
    pub use unresolved; // { dg-error ".E0432." "" { target *-*-* } }

    fn f() {
        let unresolved = 0; // OK
    }
}

fn main() {
    match 0u8 {
        m::unresolved => {} // OK
        m::unresolved(..) => {} // OK
        m::unresolved{..} => {} // OK
    }
}

