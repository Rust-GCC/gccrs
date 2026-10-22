fn main() {
    return
        { return () }
// { dg-error ".E0282." "" { target *-*-* } .-1 }
    ()
    ;
}

