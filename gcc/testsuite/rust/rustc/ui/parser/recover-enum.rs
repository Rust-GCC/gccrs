fn main() {
    enum Test {
        Very // { help "" "" { target *-*-* } }
        Bad(usize) // { help "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
        Stuff { a: usize } // { help "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
        Here
// { dg-error "" "" { target *-*-* } .-1 }
    }
}

