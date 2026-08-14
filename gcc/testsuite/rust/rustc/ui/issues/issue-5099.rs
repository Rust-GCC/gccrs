trait B <A> {
    fn a() -> A {
        this.a // { dg-error ".E0425." "" { target *-*-* } }
    }
    fn b(x: i32) {
        this.b(x); // { dg-error ".E0425." "" { target *-*-* } }
    }
    fn c() {
        let _ = || this.a; // { dg-error ".E0425." "" { target *-*-* } }
    }
}

fn main() {}

