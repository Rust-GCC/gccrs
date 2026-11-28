mod a {}

macro_rules! m {
    () => {
        use a::$crate; // { dg-error ".E0432." "" { target *-*-* } }
        use a::$crate::b; // { dg-error ".E0433." "" { target *-*-* } }
        type A = a::$crate; // { dg-error ".E0433." "" { target *-*-* } }
    }
}

m!();

fn main() {}

