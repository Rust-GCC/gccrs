// see #9186

enum Bar<T> { What } // { dg-error ".E0392." "" { target *-*-* } }

fn foo<T>() {
    static a: Bar<T> = Bar::What;
// { dg-error ".E0401." "" { target *-*-* } .-1 }
}

fn main() {
}

