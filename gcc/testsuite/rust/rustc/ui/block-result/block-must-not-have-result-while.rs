fn main() {
    while true { // { dg-warning "" "" { target *-*-* } }
        true // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
    }
}

