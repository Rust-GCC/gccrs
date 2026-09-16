use core::default; // { dg-error ".E0432." "" { target *-*-* } }

fn main() {
    let _: u8 = ::core::default::Default(); // { dg-error ".E0433." "" { target *-*-* } }
}

