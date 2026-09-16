fn foo<T>(t: T) {
    || { t; t; }; // { dg-error ".E0382." "" { target *-*-* } }
}

fn main() {}

