fn main() {
    struct Test {
        Very
        Bad // { dg-error "" "" { target *-*-* } }
        Stuff
    }
}

