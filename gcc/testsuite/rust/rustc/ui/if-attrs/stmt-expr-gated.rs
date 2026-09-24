fn main() {
    let _ = #[deny(warnings)] if true { // { dg-error ".E0658." "" { target *-*-* } }
    } else if false {
    } else {
    };
}

