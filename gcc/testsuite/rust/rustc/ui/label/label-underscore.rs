fn main() {
    '_: loop { // { dg-error "" "" { target *-*-* } }
        break '_ // { dg-error "" "" { target *-*-* } }
    }
}

