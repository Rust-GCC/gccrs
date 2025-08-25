pub fn main() {
    'a: { // { dg-error ".E0658." "" { target *-*-* } }
        break 'a;
    }
}

