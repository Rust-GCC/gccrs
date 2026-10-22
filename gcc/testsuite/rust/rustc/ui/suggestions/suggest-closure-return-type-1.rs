fn main() {
    let _v = || -> _ { [] }; // { dg-error ".E0282." "" { target *-*-* } }
}

