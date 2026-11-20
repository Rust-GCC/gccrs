fn main() {
    {
        if (foo) => {} // { dg-error "" "" { target *-*-* } }
    }
    {
        if (foo)
            bar; // { dg-error "" "" { target *-*-* } }
    }
}

